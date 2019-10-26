# Movie API


## Postgresql database

### Debian/Ubuntu

 * databáze a dev library:

```
    sudo apt-get install postgresql libpq-dev
```

### Mac OS

Je možné použít [postgreapp](http://postgresapp.com/), případně instalace přes homebrew:

    brew install postgres

## Vytvoření databáze

    $ sudo -s
    $ su - postgres
    $ psql
    psql (9.1.10)

    postgres=# CREATE USER movies WITH PASSWORD 'm0vi3zzrulezz!';
    postgres=# ALTER USER movies CREATEDB;

přihlašovací údaje k databázi naleznete v `config/database.yml`

V rámci tohoto úkolu budeme používat jenom databázi `movies_development`.

## Příprava projektu

Nejdříve je potřeba nainstalovat závislosti (ze složky projektu)

    $ bundle install

poté vyzkoušíme připojení na databázi:

    $ bundle exec rake db:create

### Problémy

#### Is the server running locally and accepting connections on Unix domain socket "/var/run/postgresql/.s.PGSQL.5432"?

Výchozí nastavení je připojení pre UNIX socket, které musí být nakonfigurováno v `/etc/postgresql/9.3/main/postgresql.conf`, kde zkontrolujte, že server poslouchá na portu `port = 5432` a `unix_socket_directories` odpovídá očekávané hodnotě.

Pokud chcete použít připojení přes TCP, je potřeba specifikovat port v `config/database.yml`, tj.:

    development:
      <<: *production
      database: movies_development
      port: 5432
      host: 'localhost'

#### PG::ConnectionBad: FATAL:  Peer authentication failed for user "movies"

PostgreSQL očekává, že každý uživatel bude mít systémový účet. Pokud tomu tak není
je potřeba upravit `pg_hba.conf`, pravděpodobně v `/etc/postgresql/9.3/main/pg_hba.conf`:

    local   all             movies                                  md5

poslední sloupec znamená, že uživatel `movies` se bude autorizovat pomocí hesla.

## Migrace databáze

Pro přístup k databázi používáme ActiveRecord (který je jednou z komponent z Rails, viz [dokumentace Rails](http://guides.rubyonrails.org/active_record_basics.html)). V tomto projektu nemáme k dispozici generátory (scaffold), takže budeme vytvářet migrace ručně.

V adresáři `db/migrate/` vytvořte soubory které začínají číselným id, např. `001_create_movies.rb`


Databázové typy v migracích

        |   Rails    |  postgresql  |
        |------------|:-------------|
        |:binary     | bytea        |
        |:boolean    | boolean      |
        |:date       | date         |
        |:datetime   | timestamp    |
        |:decimal    | decimal      |
        |:float      | float        |
        |:integer    | integer      |
        |:string     | *            |
        |:text       | text         |
        |:time       | time         |
        |:timestamp  | timestamp    |


Samotná migrace používá následující DSL, jedná se je třídu, která dědí od `ActiveRecord::Migration`

```ruby
class CreateUsers < ActiveRecord::Migration

  def change
    create_table :users do |t|
      t.string :username
      # kazdy radek je jeden sloupec v tabulce
    end
  end

end
```

Soubor s migrací by měl být umístěn v `db/migrate` a pojmenován `{poradove cislo}_create_users.rb`, tj. camel case název třídy převedeme na podtržítka v názvech souborů, tj.

    db/migrate/001_create_users.rb


Je potřeba vytvořit následující modely:

  * movie
  * occupation
  * user
  * rating

Do databáze budeme nahrávat data z [MovieLens datasetu](http://files.grouplens.org/datasets/movielens/ml-100k-README.txt)

    u.data     -- The full u data set, 100000 ratings by 943 users on 1682 items.
                  Each user has rated at least 20 movies.  Users and items are
                  numbered consecutively from 1.  The data is randomly
                  ordered. This is a tab separated list of
                 user id | item id | rating | timestamp.
                  The time stamps are unix seconds since 1/1/1970 UTC

    u.item     -- Information about the items (movies); this is a tab separated
                  list of
                  movie id | movie title | release date | video release date |
                  IMDb URL | unknown | Action | Adventure | Animation |
                  Children's | Comedy | Crime | Documentary | Drama | Fantasy |
                  Film-Noir | Horror | Musical | Mystery | Romance | Sci-Fi |
                  Thriller | War | Western |
                  The last 19 fields are the genres, a 1 indicates the movie
                  is of that genre, a 0 indicates it is not; movies can be in
                  several genres at once.
                  The movie ids are the ones used in the u.data data set.

    u.genre    -- A list of the genres.

    u.user     -- Demographic information about the users; this is a tab
                  separated list of
                  user id | age | gender | occupation | zip code
                  The user ids are the ones used in the u.data data set.



## Import dat


pokud máte správně nastavený přístup k databázi a vytvořené migrace, můžeme spustit aktualizovat databázi:

    $ bundle exec rake db:migrate


Pro import používáme `rake` úlohy, které naleznete v `lib/tasks/*`. Je potřeba doplnit načítání dat z textových souborů a ukládání záznamů do databáze.


Samotný import dat:

    $ bundle exec rake data:fetch

Import dat může trvat až několik minut. Vkládáme do DB cca 100tis. záznamů.

## k-NN - k-nearest neighbours

V rámci databáze uživatelů chceme najít `k` nejpodobnějších uživatelů. Patrně nejjednoduší
možný algoritmus je k-NN. Vezmeme vektor hodnocení daného uživate a potom počítáme postupně
vzdálenost proti všem ostatním (n-1) uživatelům a nakonec vrátíme `k` nejpodobnějších uživatelů.

Na počítání podobnosti se často používá Cosinova vzdálenost, případně Euclidovská vzdálenost.

    u = [5, 4, 5, 5, 2, 4, 5, 5, 3, 3, 2, 5, 2, 2, 1, 5]
    v = [3, 1, 1, 1, 1, 2, 1, 3, 1, 2, 2, 2, 5, 2, 1, 4]

    eucl_dist(u, v) = 9.695359714832659

    cosine_dist(u, v) = 0.188985

V rámci databáze filmů se často stává, že uživatelé ohodnotili pouze malé procento
stejných filmů. Pokud bychom porovnávali pouze filmy hodnocené oběmi uživateli
dostali bychom hodně nepřesné výsledky, např. podobní uživatelé by byli ti, kteří
ohodnotili jediný společný film a dali mu stejné hodnocení.

Jedním z přístupů jak tomu zabránit je normalizace na sjednocené množině filmů
a u filmů, které jeden z uživatelů neviděl doplníme 0

      u = [0, 5, 3, 2, 0, 1, 0]
      v = [1, 3, 0, 0, 4, 0, 1]

Tento přístup používá refernční řešení (očekávané řazení v testech). U Euclidovské vzdálenosti
normalizujeme výsledek délkou vektoru.

## Webserver

Webserver na otestování funkčnosti v prohlížeči spustíme pomocí příkazu:

    $ foreman start

Následně můžeme zkontrolovat naši aplikaci

    http://localhost:5000/status

API by mělo odpovídat na řadu dotazů (viz testy), např.:


    GET http://localhost:5000/v1/movies?limit=2

# Testování zátěže serveru

Server by měl být schopný vyřizovat souběžně mnoho requestů, na otestování zátěže se typicky používá Apache Benchmark tool.

pro Debian/Ubuntu:

  $ sudo apt-get install apache2-utils

Samotný test posílá současně (parametr `-c`) 220 požadavků na server. `-n` je celkový počet požadavků.

  $ ab -c 120 -n 1000 http://localhost:5000/v1/stress/pretend

# TL;DR (Too Long Didn't Read it)

Pro rychlé čtenáře ještě jednou postup v příkazech:

  1. `bundle install`
  2. `bundle exec rake db:create`
  3. `bundle exec rake db:migrate`
  4. `bundle exec rake data:fetch`
  5. `bundle exec rake spec`

