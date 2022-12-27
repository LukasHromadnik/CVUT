//
//  SingleCannonState.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import Foundation

class SingleCannonState: CannonState {
    
    func shoot(from cannon: Cannon) {
        let missile = Model.instance.factory.createMissile()
        Model.instance.missiles.append(missile)
        missile.accept(Model.instance.scene)
        Model.instance.set(cannon.position, for: missile)
        missile.move(with: cannon.angle.value, force: cannon.force.value)
    }
    
}
