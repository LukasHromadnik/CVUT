//
//  Memento.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit

enum GameMode: String {
    case simple
    case realistic
}

protocol MementoProtocol { }

protocol Caretaker {
    var enemyPositions: [CGPoint] { get set }
    var cannonPosition: CGPoint { get set }
    var score: Int { get set }
    var mode: GameMode { get set }
}

class Memento: NSObject, NSCoding, Caretaker, MementoProtocol {
    
    static let defaultsKey = "memento"

    var enemyPositions = [CGPoint]()
    var cannonPosition: CGPoint = .zero
    var score = 0
    var date = Date()
    var mode = GameMode.simple
    
    override init() {
        super.init()
    }
    
    func save() {
        date = Date()
//        UserDefaults.standard.set(self, forKey: Memento.defaultsKey)
        (UIApplication.shared.delegate as! AppDelegate).memento = self
    }
    
    func load() {
//        guard let memento = UserDefaults.standard.object(forKey: Memento.defaultsKey) as? Memento else { return }
        guard let memento = (UIApplication.shared.delegate as? AppDelegate)?.memento else { return }
        enemyPositions = memento.enemyPositions
        cannonPosition = memento.cannonPosition
        score = memento.score
        mode = memento.mode
        date = memento.date
    }
    
    // MARK: - NSCoding
    
    required init(coder aDecoder: NSCoder) {
        enemyPositions = aDecoder.decodeObject(forKey: "enemyPositions") as! [CGPoint]
        cannonPosition = aDecoder.decodeObject(forKey: "cannonPosition") as! CGPoint
        score = aDecoder.decodeObject(forKey: "score") as! Int
        mode = aDecoder.decodeObject(forKey: "mode") as! GameMode
        date = aDecoder.decodeObject(forKey: "date") as! Date
    }
    
    func encode(with coder: NSCoder) {
        coder.encode(enemyPositions, forKey: "enemyPositions")
        coder.encode(cannonPosition, forKey: "cannonPosition")
        coder.encode(score, forKey: "score")
        coder.encode(mode, forKey: "mode")
        coder.encode(date, forKey: "date")
    }

}
