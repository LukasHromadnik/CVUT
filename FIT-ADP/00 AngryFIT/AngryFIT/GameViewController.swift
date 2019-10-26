//
//  GameViewController.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 29.11.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit
import SpriteKit

class GameViewController: BaseViewController, ModelObservable {
    
    weak var gameView: SKView!
    weak var missileModeControl: UISegmentedControl!
    weak var forceSlider: UISlider!
    weak var forceLabel: UILabel!
    weak var angleSlider: UISlider!
    weak var angleLabel: UILabel!
    weak var gravitySlider: UISlider!
    weak var gravityLabel: UILabel!
    
    var memento: Memento?
    
    // MARK: - Initialization
    
    init(factory: GameObjectsFactory, memento: Memento? = nil) {
        super.init(nibName: nil, bundle: nil)
        
        Model.setup(with: factory)
        self.memento = memento
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: - Controller lifecycle

    override func viewDidLoad() {
        super.viewDidLoad()
        
        navigationItem.title = "Score: 0"
        
        navigationItem.rightBarButtonItem = UIBarButtonItem(title: "Save", style: .plain, target: self, action: #selector(saveAction(_:)))
        
        setupGameView()
        setupRecognizers()
        setupObservers()
        setupGameScene()
        
        Model.instance.add(observer: self)
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        
        Model.instance.loadDefaults()
        
        guard let memento = memento else { return }
        Model.instance.loadState(from: memento)
    }
    
    // MARK: - Actions

    @objc
    private func panGestureAction(_ recognizer: UIPanGestureRecognizer) {
        Model.instance.updateCannonPosition(with: recognizer.translation(in: gameView))
        
        recognizer.setTranslation(.zero, in: gameView)
    }
    
    @objc
    private func tapGestureAction(_ recognizer: UITapGestureRecognizer) {
        Model.instance.fire()
    }
    
    @objc
    private func forceSliderValueChanged(_ slider: UISlider) {
        Model.instance.update(force: Int(slider.value))
    }
    
    @objc
    private func angleSliderValueChanged(_ slider: UISlider) {
        Model.instance.update(angle: Int(slider.value))
    }
    
    @objc
    private func gravitySliderValueChanged(_ slider: UISlider) {
        Model.instance.update(gravity: Int(slider.value))
    }
    
    @objc
    private func missileModeControlValueChanged(_ control: UISegmentedControl) {
        Model.instance.changeCannonState(control.selectedSegmentIndex)
    }
    
    @objc
    private func saveAction(_ sender: UIBarButtonItem) {
        Model.instance.saveState()
    }
    
    // MARK: - Components setup
    
    override func loadView() {
        super.loadView()
        
        let missileModeControl = UISegmentedControl(items: ["Single mode", "Double mode"])
        missileModeControl.selectedSegmentIndex = 0
        missileModeControlValueChanged(missileModeControl)
        self.missileModeControl = missileModeControl
        
        let model = Model.instance
        
        let force = model.cannon.force
        let forceView = GamePropertyView()
        forceView.titleLabel.text = force.name
        forceView.slider.minimumValue = Float(force.min)
        forceView.slider.maximumValue = Float(force.max)
        forceView.slider.value = Float(force.value)
        forceView.valueLabel.text = "\(force.value)"
        self.forceSlider = forceView.slider
        self.forceLabel = forceView.valueLabel
        
        let angle = model.cannon.angle
        let angleView = GamePropertyView()
        angleView.titleLabel.text = angle.name
        angleView.slider.minimumValue = Float(angle.min)
        angleView.slider.maximumValue = Float(angle.max)
        angleView.slider.value = Float(angle.value)
        angleView.valueLabel.text = "\(angle.value)"
        self.angleSlider = angleView.slider
        self.angleLabel = angleView.valueLabel
        
        let gravity = model.gravity
        let gravityView = GamePropertyView()
        gravityView.titleLabel.text = gravity.name
        gravityView.slider.minimumValue = Float(gravity.min)
        gravityView.slider.maximumValue = Float(gravity.max)
        gravityView.slider.value = Float(gravity.value)
        gravityView.valueLabel.text = "\(gravity.value)"
        self.gravitySlider = gravityView.slider
        self.gravityLabel = gravityView.valueLabel
        
        let bottomView = UIStackView(arrangedSubviews: [missileModeControl, forceView, angleView, gravityView])
        bottomView.axis = .vertical
        bottomView.spacing = 5
        view.addSubview(bottomView)
        bottomView.snp.makeConstraints { (make) in
            make.leading.trailing.equalTo(0).inset(15)
            make.bottom.equalTo(view.safeAreaLayoutGuide.snp.bottom).inset(15)
        }
        
        let divider = UIView()
        divider.backgroundColor = .gray
        view.addSubview(divider)
        divider.snp.makeConstraints { make in
            make.leading.trailing.equalTo(0)
            make.bottom.equalTo(bottomView.snp.top).offset(-15)
            make.height.equalTo(1)
        }
        
        let gameView = SKView()
        view.addSubview(gameView)
        gameView.snp.makeConstraints { (make) in
            make.top.equalTo(66)
            make.leading.trailing.equalTo(0)
            make.bottom.equalTo(divider.snp.top)
        }
        self.gameView = gameView
    }
    
    func setupGameView() {
        gameView.showsFPS = true
        gameView.showsNodeCount = true
        gameView.ignoresSiblingOrder = true
        gameView.layoutIfNeeded()
    }
    
    func setupRecognizers() {
        let panGesture = UIPanGestureRecognizer(target: self, action: #selector(panGestureAction(_:)))
        gameView.addGestureRecognizer(panGesture)
        
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(tapGestureAction(_:)))
        gameView.addGestureRecognizer(tapGesture)
    }
    
    func setupObservers() {
        forceSlider.addTarget(self, action: #selector(forceSliderValueChanged(_:)), for: .valueChanged)
        angleSlider.addTarget(self, action: #selector(angleSliderValueChanged(_:)), for: .valueChanged)
        gravitySlider.addTarget(self, action: #selector(gravitySliderValueChanged(_:)), for: .valueChanged)
        missileModeControl.addTarget(self, action: #selector(missileModeControlValueChanged(_:)), for: .valueChanged)
    }
    
    func setupGameScene() {
        let scene = GameScene(size: gameView.frame.size)
        Model.instance.scene = scene
        scene.scaleMode = .resizeFill
        gameView.presentScene(scene)
    }

    // MARK: - ModelObservable
    
    func property(_ property: Property, changed value: GameProperty.Value) {
        switch property {
        case .Force:
            forceLabel.text = "\(value)"
            forceSlider.value = Float(value)
        case .Angle:
            angleLabel.text = "\(value)"
            angleSlider.value = Float(value)
        case .Gravity:
            gravityLabel.text = "\(value)"
            gravitySlider.value = Float(value)
        case .Score:
            navigationItem.title = "Score: \(value)"
        }
    }
    
    // MARK: - Memento
    
    func loadGame(from memento: MementoProtocol) {
        Model.instance.loadState(from: memento)
    }
    
}
