//
//  MissileStrategy.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit

protocol MissileStrategy {
    func move(_ missile: Missile, angle: GameProperty.Value, force: GameProperty.Value)
}
