//
//  GameObject.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit

protocol GameObject: GameObjectVisitable {
    var imageName: String { get }
    var position: CGPoint { get set }
}

extension GameObject {
    
    var origin: CGPoint? {
        guard position != .zero else { return nil }
        return CGPoint(x: position.x - size.width / 2, y: position.y - size.height / 2)
    }
    
    var size: CGSize {
        return UIImage(named: self.imageName)?.size ?? .zero
    }
    
    var frame: CGRect? {
        guard let origin = origin else { return nil }
        return CGRect(origin: origin, size: size)
    }

}
