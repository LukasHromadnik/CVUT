//
//  ModelObservable.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import Foundation

protocol ModelObservable: AnyObject {
    func property(_ property: Property, changed value: GameProperty.Value)
}
