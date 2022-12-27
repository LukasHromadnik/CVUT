//
//  DoubleCannonState.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import Foundation

class DoubleCannonState: CannonState {
    
    func shoot(from cannon: Cannon) {
        let angle = cannon.angle.value
        let angles = [angle - 5, angle + 5]
        angles.forEach {
            let missile = Model.instance.factory.createMissile()
            Model.instance.missiles.append(missile)
            missile.position = cannon.position
            missile.accept(Model.instance.scene)
            missile.move(with: $0, force: cannon.force.value)
        }
    }
    
}
