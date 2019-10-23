print_values([]).
print_values(Values) :-
    [Value|Other] = Values,
    writeln(Value),
    print_values(Other),
    !.

info(Values) :- info(Values, false).
info(Values, Delimiter) :-
    (
        use_debug(),
        print_values(Values),
        delimiter(Delimiter),
        !
    ;
        true
    ).

delimiter(UseDelimiter) :- 
    (
        UseDelimiter,
        use_debug(),
        writeln("===================="),
        !
    ;
        true
    ).

use_debug() :- false.
