//
//  StartViewController.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 12.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit

class StartViewController: BaseViewController, UITableViewDataSource, UITableViewDelegate {
    
    weak var simpleModeButton: UIButton!
    weak var realisticModeButton: UIButton!
    weak var tableView: UITableView!
    
    var mementos = [Memento]()
    
    // MARK: - Controller lifecycle
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        navigationItem.title = "AngryFIT"
        
        simpleModeButton.addTarget(self, action: #selector(simpleModeButtonAction(_:)), for: .touchUpInside)
        realisticModeButton.addTarget(self, action: #selector(realisticModeButtonAction(_:)), for: .touchUpInside)
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        
//        if let memento = UserDefaults.standard.object(forKey: Memento.defaultsKey) as? Memento {
//            mementos.append(memento)
//        }
        if let memento = (UIApplication.shared.delegate as? AppDelegate)?.memento {
            mementos = [memento]
        }
        tableView.reloadData()
    }
    
    // MARK: - Actions

    @objc
    private func simpleModeButtonAction(_ button: UIButton) {
        let gameController = GameViewController(factory: SimpleGameObjectsFactory())
        show(gameController, sender: self)
    }

    @objc
    private func realisticModeButtonAction(_ button: UIButton) {
        let gameController = GameViewController(factory: RealisticGameObjectsFactory())
        show(gameController, sender: self)
    }
    
    // MARK: - Components setup
    
    override func loadView() {
        super.loadView()
        
        let simpleModeButton = UIButton()
        simpleModeButton.setTitle("Simple mode", for: .normal)
        simpleModeButton.setTitleColor(.blue, for: .normal)
        self.simpleModeButton = simpleModeButton
        
        let realisticModeButton = UIButton()
        realisticModeButton.setTitle("Realistic mode", for: .normal)
        realisticModeButton.setTitleColor(.blue, for: .normal)
        self.realisticModeButton = realisticModeButton
        
        let contentView = UIStackView(arrangedSubviews: [simpleModeButton, realisticModeButton])
        contentView.spacing = 15
        contentView.axis = .vertical
        view.addSubview(contentView)
        contentView.translatesAutoresizingMaskIntoConstraints = false
        NSLayoutConstraint.activate([
            contentView.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: 15),
            contentView.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -15),
            contentView.topAnchor.constraint(equalTo: view.topAnchor, constant: 100)
        ])
        
        let tableView = UITableView(frame: .zero, style: .grouped)
        tableView.dataSource = self
        tableView.delegate = self
        view.addSubview(tableView)
        tableView.translatesAutoresizingMaskIntoConstraints = false
        NSLayoutConstraint.activate([
            tableView.topAnchor.constraint(equalTo: contentView.bottomAnchor, constant: 15),
            tableView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            tableView.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            tableView.bottomAnchor.constraint(equalTo: view.bottomAnchor)
        ])
        self.tableView = tableView
    }
    
    // MARK: - UITableViewDataSource
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return mementos.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = UITableViewCell(style: .default, reuseIdentifier: nil)
        cell.textLabel?.text = "\(mementos[indexPath.row].date)"
        
        return cell
    }
    
    // MARK: - UITableViewDelegate
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let memento = mementos[indexPath.row]
        
        let factory: GameObjectsFactory = memento.mode == .simple ? SimpleGameObjectsFactory() : RealisticGameObjectsFactory()
        let gameController = GameViewController(factory: factory, memento: memento)
        show(gameController, sender: self)
    }
    
}
