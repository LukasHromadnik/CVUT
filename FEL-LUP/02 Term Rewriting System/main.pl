:- [debug].

is_not_empty(List) :- [_|_] = List.

use_rule(Rule, Input, Out) :-
    info(["use_rule", Rule, Input], true),
    copy_term(Rule, rule(Lhs, Rhs)),
    Lhs = Input,
    Out = Rhs.

apply_rules(Rules, Input, Out) :-
    info(["apply_rules", Rules, Input], true),
    [Rule|Other] = Rules,
    (
        use_rule(Rule, Input, Out),
        !
        ;
        apply_rules(Other, Input, Out)
    ).

process_args(Rules, Args, Out) :-
    info(["process_args", "Args", Args]),
    [Arg|Other] = Args,
    info(["Arg", Arg, "Other", Other], true),
    (
        apply_rules(Rules, Arg, Out1),
        info(["apply_rules out", Out1], true),
        Out = [Out1|Other]
        ;
        my_match(Rules, Arg, Out1),
        info(["apply_rules out", Out1], true),
        Out = [Out1|Other]
        ;
        is_not_empty(Other),
        process_args(Rules, Other, Out1),
        info(["process_args out", Out1, Other]),
        Out = [Arg|Out1]
    ).

my_match(Rules, Input, Out) :-
    (
        info(["my_match", Input]),
        Input =.. [Name|Args],
        info(["Name", Name, "Args", Args], true),
        process_args(Rules, Args, Out1),
        is_not_empty(Out1),
        Out =.. [Name|Out1]
    ).

match(Rules, Input, Out) :-
    info(["match", Input], true),
    (
        apply_rules(Rules, Input, Out)
    ;
        my_match(Rules, Input, Out)
    ;
        Out = Input
    ).

% match([rule(f(X), g(X))], f(f(a)), Out).
% match([rule(f(X), g(X)), rule(g(X), f(X))], f(g(a)), Out).
% match([rule(f(X), g(X))], f(f(f(a))), Out).
% match([rule(not(not(X)), X), rule(not(or(X,Y)), and(not(X),not(Y))), rule(not(and(X,Y)), or(not(X),not(Y))), rule(or(and(X,Y),Z), and(or(X,Z),or(Y,Z))), rule(or(X,and(Y,Z)), and(or(X,Y),or(X,Z)))], and(a, not(and(not(b), c))), Out).
% match([rule(not(not(X)), X)], and(not(not(a)), or(not(b), c)), Out).
% match([rule(not(not(X)), X)], and(or(not(b), c), not(not(a))), Out).

fixpoint(Rules, Input, Out) :-
    match(Rules, Input, Out1),
    !,
    (
        Out1 == Input,
        Out = Input
    ;
        fixpoint(Rules, Out1, Out)
    ),
    !.

% fixpoint([rule(f(X), g(X)), rule(f(X), h(X))], f(f(a)), Out).
% fixpoint([rule(f(X), g(X)), rule(f(X), h(X))], b(b(a)), Out).
% fixpoint([rule(f(X), g(X))], f(f(f(a))), Out).
% fixpoint([rule(not(not(X)), X), rule(not(or(X,Y)), and(not(X),not(Y))), rule(not(and(X,Y)), or(not(X),not(Y))), rule(or(and(X,Y),Z), and(or(X,Z),or(Y,Z))), rule(or(X,and(Y,Z)), and(or(X,Y),or(X,Z)))], not(or(not(a), and(not(b),c))), Out).
% fixpoint([rule([[A|B]|C], [A|[B|C]]), rule([[]|X], X)], [[a], b, [[[c], d]]], Out).
