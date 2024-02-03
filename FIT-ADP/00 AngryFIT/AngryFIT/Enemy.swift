//
//  Enemy.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 12.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit

class Enemy: GameObject {
    
    let imageName: String = {
        let imageNumber = arc4random() % 2 + 1
        return "enemy\(imageNumber)"
    }()
    
    var position: CGPoint = .zero
    
    // MARK: - GameObjectVisitable
    
    func accept(_ visitor: GameObjectVisitor) {
        visitor.visit(self)
    }
    
}
