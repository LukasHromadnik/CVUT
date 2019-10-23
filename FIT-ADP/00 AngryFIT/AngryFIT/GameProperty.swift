//
//  GameProperty.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import Foundation

class GameProperty {
    typealias Value = Int
    
    let name: String
    var value: Value
    let min: Value
    let max: Value
    
    init(name: String, value: Value, min: Value, max: Value) {
        self.name = name
        self.value = value
        self.min = min
        self.max = max
    }
    
}
