# EM - Expectation Maximisation Algorithm

## Assignment 1

E-step deduction: Please see attachment `E-step.pdf`

M-step deduction of the item a): Please see attachment `M-step.pdf`

## Assignment 2

* K-Means error: 16.4192 %
* GMM error: 11.9911 %

Both approaches were evaluated on the full set of images and the final result was obtained as an arithmetic mean.

Better result was given by GMM approach. This result is probably caused by better segmentation of the pixels where in GMM every pixel has a probability that it belongs to one of the segments. But in K-Means there are no probabilities, only hard assignemnts to one segment. Another reason can be that Gaussian distribution is more flexible in the shape. K-Means is just circle around the center.

## Assignment 3

* EM error: 1.0645 %

Comparing to the baseline models the EM algorithm has a better error. This value can be further improved. The final value was obtained by using condition to end the main cycle.

The final shape model is clearer than segmentations from the baseline models.

The final errors for each image:

* hand_00: 1.3577854671280276 %
* hand_01: 1.1737024221453287 %
* hand_02: 0.6131487889273356 %
* hand_03: 1.645674740484429 %
* hand_04: 0.8899653979238755 %
* hand_05: 1.582006920415225 %
* hand_06: 0.546712802768166 %
* hand_07: 0.9411764705882352 %
* hand_08: 1.0740484429065744 %
* hand_09: 1.4062283737024222 %
* hand_10: 0.22422145328719725 %
* hand_11: 1.3923875432525952 %
* hand_12: 0.584083044982699 %
* hand_13: 0.7003460207612457 %
* hand_14: 1.0325259515570935 %
* hand_15: 1.015916955017301 %
* hand_16: 1.6235294117647059 %
* hand_17: 1.6069204152249132 %
* hand_18: 1.5349480968858131 %
* hand_19: 0.4221453287197232 %
* hand_20: 0.6463667820069204 %
* hand_21: 0.7944636678200692 %
* hand_22: 1.3301038062283737 %
* hand_23: 1.1294117647058823 %
* hand_24: 1.346712802768166 %
* hand_25: 1.4214532871972319 %
* hand_26: 0.9231833910034601 %
* hand_27: 1.5847750865051902 %
* hand_28: 1.0878892733564014 %
* hand_29: 1.6498269896193771 %
* hand_30: 1.678892733564014 %
* hand_31: 1.0961937716262975 %
* hand_32: 1.2470588235294118 %
* hand_33: 0.93840830449827 %
* hand_34: 0.3875432525951557 %
* hand_35: 0.754325259515571 %
* hand_36: 1.6027681660899653 %
* hand_37: 0.3889273356401384 %
* hand_38: 0.560553633217993 %
* hand_39: 1.511418685121107 %
* hand_40: 0.43737024221453286 %
* hand_41: 0.6048442906574395 %
* hand_42: 0.39031141868512115 %
* hand_43: 0.9176470588235295 %
* hand_44: 0.754325259515571 %
* hand_45: 1.5875432525951558 %
* hand_46: 1.87681660899654 %
* hand_47: 1.835294117647059 %
* hand_48: 1.0768166089965396 %
* hand_49: 0.3003460207612456 %