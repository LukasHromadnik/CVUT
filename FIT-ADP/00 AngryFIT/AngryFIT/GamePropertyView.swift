//
//  GamePropertyView.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit

class GamePropertyView: UIView {
    
    weak var titleLabel: UILabel!
    weak var slider: UISlider!
    weak var valueLabel: UILabel!
    
    // MARK: - Initialization
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        setup()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    func setup() {
        let titleLabel = UILabel()
        addSubview(titleLabel)
        titleLabel.snp.makeConstraints { (make) in
            make.top.leading.bottom.equalTo(0)
        }
        self.titleLabel = titleLabel
        
        let valueLabel = UILabel()
        addSubview(valueLabel)
        valueLabel.snp.makeConstraints { (make) in
            make.top.trailing.bottom.equalTo(0)
        }
        self.valueLabel = valueLabel
        
        let slider = UISlider()
        addSubview(slider)
        slider.snp.makeConstraints { (make) in
            make.top.bottom.equalTo(0)
            make.leading.equalTo(titleLabel.snp.trailing).offset(15)
            make.trailing.equalTo(valueLabel.snp.leading).offset(-15)
        }
        self.slider = slider
    }
    
}
