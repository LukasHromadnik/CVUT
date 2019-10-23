# Eliza

Firsly install all dependencies:

     $ bundle install

Then run tests with:

     $ bundle exec rspec spec

Your task is to write an ELIZA programme.

ELIZA emulates a Rogerian psychotherapist.

ELIZA has almost no intelligence whatsoever, only tricks like string substitution and canned responses based on keywords. Yet when the original ELIZA first appeared in the 60's, some people actually mistook her for human. The illusion of intelligence works best, however, if you limit your conversation to talking about yourself and your life.

## Client

In `eliza_client.rb` fix connection, so that it's using method `connect` from `SocketClient` module.

## Server

Write ELIZA logic, so that it will pass all tests. You should look for certain keywords, have a look how to use regular expressions in ruby case statement.


## Automatic testing

You can try automated testing with `guard`. Simply run

    $ bundle exec guard

Guard should be watching changes in your files, if you want to run all tests type "a" into Guard console.


## Standalone server

You can try standalone ELIZA server

    $ ruby server.rb

Then from different terminal connect to the server

    $ telnet 127.0.0.1 12345

Now, you can communicate with ELIZA bot.
