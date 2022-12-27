//
//  Model.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit
import SpriteKit

enum Property {
    case Force, Angle, Gravity, Score
}

class Model {
    
    static let numberOfEnemies = 5
    
    // Singleton
    
    static var instance: Model {
        guard let model = Model._instance else { assertionFailure(); return Model(factory: SimpleGameObjectsFactory()) }
        
        return model
    }
    
    private static var _instance: Model? = nil
    
    static func setup(with factory: GameObjectsFactory) {
        let model = Model(factory: factory)
        Model._instance = model
    }
    
    init(factory: GameObjectsFactory) {
        self.factory = factory
        self.enemies = (0..<Model.numberOfEnemies).map { _ in factory.createEnemy() }
    }
    
    var factory: GameObjectsFactory!
    
    let cannon = Cannon()
    var enemies = [Enemy]()
    var missiles = [Missile]()
    
    let cannonStates: [CannonState] = [SingleCannonState(), DoubleCannonState()]
    
    var gameObjects: [GameObject] {
        return [cannon] + (enemies as [GameObject])
    }
    
    weak var scene: GameScene!
    
    // Observing
    private var observers = [ModelObservable]()
    
    // Properties
    let gravity = GameProperty(name: "Gravity", value: 5, min: 0, max: 12)
    let score = GameProperty(name: "Score", value: 0, min: 0, max: -1)
    
    func loadDefaults() {
        update(gravity: gravity.value)
    }
    
    // MARK: - Cannon position
    
    func set(_ position: CGPoint, for cannon: Cannon) {
        cannon.position = position
        scene.updatePosition(for: cannon)
    }
    
    func updateCannonPosition(with translation: CGPoint) {
        let position = scene.validPosition(for: cannon, with: translation)
        set(position, for: cannon)
    }
    
    func changeCannonState(_ index: Int) {
        cannon.state = cannonStates[index]
    }
    
    // MARK: - Missile
    
    func fire() {
        cannon.fire()
    }
    
    func set(_ position: CGPoint, for missile: Missile) {
        missile.position = position
        scene.updatePosition(for: missile)
    }
    
    func run(_ action: SKAction, on missile: Missile, update: ((SKSpriteNode) -> Void)? = nil) {
        scene.run(action, on: missile, update: update)
    }
    
    func removeMissile(at index: Int) {
        missiles.remove(at: index)
    }
    
    // MARK: - Enemy
    
    func set(_ position: CGPoint, for enemy: Enemy) {
        enemy.position = position
        scene.updatePosition(for: enemy)
    }
    
    func removeEnemy(at index: Int) {
        scene.setCollisionImage(at: index)
        enemies.remove(at: index)
    }
    
    func changePosition(for enemy: Enemy) {
        let position = scene.loadRandomPosition(for: enemy)
        set(position, for: enemy)
    }
    
    func generateNewEnemy() {
        DispatchQueue.main.async { [weak self] in
            guard let strongSelf = self else { return }
            let enemy = strongSelf.factory.createEnemy()
            strongSelf.enemies.append(enemy)
            enemy.accept(strongSelf.scene)
        }
    }
    
    // MARK: - Observing
    
    func add(observer: ModelObservable) {
        observers.append(observer)
    }
    
    func remove(observer: ModelObservable) {
        observers
            .enumerated()
            .filter { $0.element === observer }
            .map { $0.offset }
            .forEach { observers.remove(at: $0) }
    }
    
    func notifyObservers(_ property: Property, changed value: GameProperty.Value) {
        observers.forEach { $0.property(property, changed: value) }
    }
    
    // MARK: - Properties changed
    
    func update(force value: Int) {
        cannon.force.value = value
        notifyObservers(.Force, changed: value)
    }
    
    func update(angle value: Int) {
        cannon.angle.value = value
        notifyObservers(.Angle, changed: value)
    }
    
    func update(gravity value: Int) {
        gravity.value = value
        notifyObservers(.Gravity, changed: value)
    }
    
    func addToScore(_ value: Int) {
        score.value += value
        notifyObservers(.Score, changed: score.value)
    }
    
    // MARK: - Memento
    
    func saveState() {
        let memento = Memento()
        memento.cannonPosition = cannon.position
        memento.enemyPositions = enemies.map { $0.position }
        memento.score = score.value
        if let _ = factory as? SimpleGameObjectsFactory {
            memento.mode = .simple
        } else {
            memento.mode = .realistic
        }
        memento.save()
    }
    
    func loadState(from memento: MementoProtocol) {
        guard let memento = memento as? Memento else { return }
        
        DispatchQueue.main.async { [weak self] in
            guard let strongSelf = self else { return }

            strongSelf.updateCannonPosition(with: memento.cannonPosition)
            
            for (index, position) in memento.enemyPositions.enumerated() {
                strongSelf.set(position, for: strongSelf.enemies[index])
            }
            
            strongSelf.addToScore(memento.score)
            
        }
        
    }
    
}
