//
//  GameScene.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 29.11.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import SpriteKit

class GameScene: SKScene, GameObjectVisitor, SKPhysicsContactDelegate, ModelObservable {
    
    weak var cannon: SKSpriteNode!
    var enemies = [SKSpriteNode]()
    var missiles = [SKSpriteNode]()
    
    override func didMove(to view: SKView) {
        super.didMove(to: view)
        
        backgroundColor = .white
        
        physicsBody = SKPhysicsBody(edgeLoopFrom: view.frame.insetBy(dx: -300, dy: -300))
        physicsBody?.categoryBitMask = GamePhysicsCategory.Boundary
        physicsBody?.contactTestBitMask = GamePhysicsCategory.Missile
        physicsBody?.collisionBitMask = GamePhysicsCategory.None
        
        physicsWorld.gravity = .zero
        physicsWorld.contactDelegate = self
        
        Model.instance.gameObjects.forEach { $0.accept(self) }
        Model.instance.add(observer: self)
    }
    
    func validPosition(for cannon: Cannon, with translation: CGPoint) -> CGPoint {
        guard let gameView = view else { return .zero }
        
        let position = cannon.position.y - translation.y
        let bottomAnchor = (cannon.size.height / 2) + 5
        let topAnchor = gameView.frame.size.height - bottomAnchor
        
        let newY: CGFloat
        if position <= bottomAnchor {
            newY = bottomAnchor
        } else if position > topAnchor {
            newY = topAnchor
        } else {
            newY = position
        }
        
        return CGPoint(x: cannon.position.x, y: newY)
    }
    
    func updatePosition(for cannon: Cannon) {
        self.cannon.position = cannon.position
    }
    
    func updatePosition(for enemy: Enemy) {
        guard let index = Model.instance.enemies.enumerated().filter({ $0.element === enemy }).map({ $0.offset }).first else { return }

        enemies[index].position = enemy.position
    }
    
    func updatePosition(for missile: Missile) {
        guard let index = Model.instance.missiles.enumerated().filter({ $0.element === missile }).map({ $0.offset }).first else { return }
        
        missiles[index].position = missile.position
    }
    
    // Enemy
    
    func loadRandomPosition(for enemy: Enemy) -> CGPoint {
        let padding = 50
        let objectFrames = Model.instance.gameObjects.map { $0.frame }.flatMap { $0 }
        let minX = Int(Model.instance.cannon.size.width) + padding
        let minY = 15
        let maxX = Int(view?.frame.size.width ?? 0) - padding
        let maxY = Int(view?.frame.size.height ?? 0) - padding
        let rangeX = UInt32(maxX - minX)
        let rangeY = UInt32(maxY - minY)
        
        var hasIntersection = true
        
        while hasIntersection {
            let positionX = arc4random() % rangeX
            let positionY = arc4random() % rangeY
            
            enemy.position = CGPoint(x: Int(positionX) + minX, y: Int(positionY) + minY)
            
            guard let enemyFrame = enemy.frame else { continue }
            
            hasIntersection = objectFrames.filter { $0.intersects(enemyFrame) }.count > 0
        }
        
        return enemy.position
    }
    
    func setCollisionImage(at index: Int) {
        let enemy = enemies[index]
        enemy.removeFromParent()
        enemies.remove(at: index)
        
        let collision = SKSpriteNode(imageNamed: "collision")
        collision.position = enemy.position
        addChild(collision)
        Timer.scheduledTimer(withTimeInterval: 1, repeats: false, block: { _ in collision.removeFromParent() })
    }
    
    func removeEnemy(at index: Int) {
        enemies[index].removeFromParent()
        enemies.remove(at: index)
    }
    
    // Missile
    
    func run(_ action: SKAction, on missile: Missile, update: ((SKSpriteNode) -> Void)? = nil) {
        guard let index = Model.instance.missiles.enumerated().filter({ $0.element === missile }).map({ $0.offset }).first else { return }
        
        let missile = missiles[index]
        
        update?(missile)
        missile.run(action)
    }
    
    // MARK: - GameObjectVisitor
    
    func visit(_ cannon: Cannon) {
        let cannonNode = SKSpriteNode(imageNamed: cannon.imageName)
        addChild(cannonNode)
        self.cannon = cannonNode
        
        let defaultPosition = CGPoint(x: (cannon.size.width / 2) + 5, y: size.height * 0.5)
        Model.instance.set(defaultPosition, for: cannon)
    }
    
    func visit(_ enemy: Enemy) {
        let enemyNode = SKSpriteNode(imageNamed: enemy.imageName)
        addChild(enemyNode)
        enemies.append(enemyNode)
        
        enemyNode.physicsBody = SKPhysicsBody(rectangleOf: enemy.size)
        enemyNode.physicsBody?.isDynamic = true
        enemyNode.physicsBody?.categoryBitMask = GamePhysicsCategory.Enemy
        enemyNode.physicsBody?.contactTestBitMask = GamePhysicsCategory.Missile
        enemyNode.physicsBody?.collisionBitMask = GamePhysicsCategory.None
        enemyNode.physicsBody?.affectedByGravity = false
        
        let position = loadRandomPosition(for: enemy)
        Model.instance.set(position, for: enemy)
    }
    
    func visit(_ missile: Missile) {
        let missileNode = SKSpriteNode(imageNamed: missile.imageName)
        missileNode.zPosition = 1
        addChild(missileNode)
        missiles.append(missileNode)
        
        missileNode.physicsBody = SKPhysicsBody(rectangleOf: missile.size)
        missileNode.physicsBody?.isDynamic = true
        missileNode.physicsBody?.categoryBitMask = GamePhysicsCategory.Missile
        missileNode.physicsBody?.contactTestBitMask = GamePhysicsCategory.Enemy | GamePhysicsCategory.Boundary
        missileNode.physicsBody?.collisionBitMask = GamePhysicsCategory.None
        missileNode.physicsBody?.usesPreciseCollisionDetection = true
        missileNode.physicsBody?.affectedByGravity = false
        
        let model = Model.instance
        model.set(model.cannon.position, for: missile)
    }
    
    // MARK: - SKPhysicsContactDelegate
    
    func didBegin(_ contact: SKPhysicsContact) {
        var firstBody: SKPhysicsBody
        var secondBody: SKPhysicsBody
        
        if contact.bodyA.categoryBitMask < contact.bodyB.categoryBitMask {
            firstBody = contact.bodyA
            secondBody = contact.bodyB
        } else {
            firstBody = contact.bodyB
            secondBody = contact.bodyA
        }

        if firstBody.categoryBitMask & GamePhysicsCategory.Enemy != 0 && secondBody.categoryBitMask & GamePhysicsCategory.Missile != 0 {
            guard let missileNode = secondBody.node as? SKSpriteNode, let enemyNode = firstBody.node as? SKSpriteNode else { return }
            missile(missileNode, didCollideWith: enemyNode)
        } else if firstBody.categoryBitMask & GamePhysicsCategory.Missile != 0 && secondBody.categoryBitMask & GamePhysicsCategory.Boundary != 0 {
            (firstBody.node as? SKSpriteNode)?.removeFromParent()
        }
    }
    
    func missile(_ missile: SKSpriteNode, didCollideWith enemy: SKSpriteNode) {
        missile.removeFromParent()
        
        if let index = missiles.enumerated().filter({ $0.element === missile }).map({ $0.offset }).first {
            missiles.remove(at: index)
            Model.instance.removeMissile(at: index)
        }
        
        if let index = enemies.enumerated().filter({ $0.element === enemy }).map({ $0.offset }).first {
            Model.instance.removeEnemy(at: index)
        }
        
        Model.instance.addToScore(10)
        Model.instance.generateNewEnemy()
    }
    
    // MARK: - ModelObservable
    
    func property(_ property: Property, changed value: GameProperty.Value) {
        switch property {
        case .Gravity:
            physicsWorld.gravity = CGVector(dx: 0, dy: -value)
        default: return
        }
    }
    
}
