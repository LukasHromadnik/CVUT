(define (domain logistics)
    (:requirements :strips)
    (:predicates
        (has ?town ?item)
        (sibling ?town1 ?town2)
        (delivered ?item)
        (free ?car)
    )
    (:action naloz
        :parameters (?item ?town ?car)
        :precondition (and
            (is-car ?car)
            (has ?town ?item)
            (has ?town ?car)
            (free ?car)
        )
        :effect (and
            (has ?car ?item)
            (not (has ?town ?item))
            (not (free ?car))
        )
    )
    (:action move-car
        :parameters (?car ?from ?to)
        :precondition (and
            (is-car ?car)
            (has ?from ?car)
            (sibling ?from ?to)
        )
        :effect (and
            (has ?to ?car)
            (not (has ?from ?car))
        )
    )
    (:action vyloz
        :parameters (?item ?town ?car)
        :precondition (and
            (is-car ?car)
            (has ?car ?item)
            (has ?town ?car)
        )
        :effect (and
            (has ?town ?item)
            (not (has ?car ?item))
            (free ?car)
        )
    )
)
