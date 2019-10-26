(define (domain logistics)
    (:requirements :strips)
    (:predicates
        (is_tile ?tile)
        (is_pos ?pos)
        (next ?pos1 ?pos2)
        (on ?pos ?tile)
        (free ?pos)
    )
    (:action move
        :parameters (?from ?to ?tile)
        :precondition (and
            (is_tile ?tile)
            (is_pos ?from)
            (is_pos ?to)
            (next ?from ?to)
            (free ?to)
            (on ?from ?tile)
        )
        :effect (and
            (free ?from)
            (on ?to ?tile)
            (not (free ?to))
            (not (on ?from ?tile))
        )
    )
)