package wumpus.agent;

import cz.agents.alite.vis.VisManager;
import cz.agents.alite.vis.layer.toggle.KeyToggleLayer;
import wumpus.world.Action;


public class DebugVis {
	public static float[][] stateValues;
    public static float[][][] stateActionValues;
    public static Action[][] policy;
    
    /**
     * Create three visualization layers:
     * 1) State-value layer (activated by pressing "s")
     * 2) wumpus.world.Action-value layer (activated by pressing "a")
     * 3) Policy layer (activted by pressing "p")
     */
    
    public static KeyToggleLayer sLayer;
    public static KeyToggleLayer aLayer;
    public static KeyToggleLayer pLayer;

    public static void initVis() {
    	
    	if (sLayer != null) {
    		VisManager.unregisterLayer(sLayer);
    	}
    	
    	if (aLayer != null) {
    		VisManager.unregisterLayer(aLayer);
    	}
    	
    	if (pLayer != null) {
    		VisManager.unregisterLayer(pLayer);
    	}
    	
    	
		sLayer = KeyToggleLayer.create("s");
		sLayer.addSubLayer(
			StateValueFunctionLayer.create(new  StateValueFunctionLayer.ValueFunctionProvider() {

				@Override
				public float[][] getValueFunction() {
					return stateValues;
				}
			}, 1));
		sLayer.setEnabled(false);
		VisManager.registerLayer(sLayer);

		aLayer = KeyToggleLayer.create("a");
		aLayer.addSubLayer(
			ActionValueFunctionLayer.create(new  ActionValueFunctionLayer.ActionValueFunctionProvider() {

				@Override
				public float[][][] getValueFunction() {
					return stateActionValues;
				}
			}, 1));
		aLayer.setEnabled(false);
		VisManager.registerLayer(aLayer);

		pLayer = KeyToggleLayer.create("p");
		pLayer.addSubLayer(
			PolicyLayer.create(new  PolicyLayer.PolicyProvider() {

				@Override
				public Action[][] getPolicy() {
					return policy;
				}
			}, 1));
		pLayer.setEnabled(false);
		VisManager.registerLayer(pLayer);
    }

	public static void setStateValues(float[][] stateValues) {
		DebugVis.stateValues = stateValues;
	}

	public static void setStateActionValues(float[][][] stateActionValues) {
		DebugVis.stateActionValues = stateActionValues;
	}

	public static void setPolicy(Action[][] policy) {
		DebugVis.policy = policy;
	}
    
    
    
    
}
