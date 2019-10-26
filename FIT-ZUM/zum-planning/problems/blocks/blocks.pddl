(define (domain blocks)
    (:requirements :strips)
    (:predicates
        (on ?x ?y)
        (distinct ?x ?y)
        (on-top ?x)
        (on-ground ?x)
    )
    (:action move
        :parameters (?what ?from ?to)
        :precondition (and
            (distinct ?what ?to)
            (on ?from ?what)
            (on-top ?what)
            (on-top ?to)
          )
        :effect (and
            (on ?to ?what)
            (on-top ?from)
            (not (on ?from ?what))
            (not (on-top ?to))
          )
    )
    (:action from-ground
        :parameters (?what ?to)
        :precondition (and
            (on-ground ?what)
            (on-top ?what)
            (on-top ?to)
          )
        :effect (and
            (on ?to ?what)
            (not (on-ground ?what))
            (not (on-top ?to))
          )
    )
    (:action to-ground
        :parameters (?what ?from)
        :precondition (and
            (on ?from ?what)
            (on-top ?what)
          )
        :effect (and
            (on-ground ?what)
            (on-top ?from)
            (not (on ?from ?what))
          )
    )
)