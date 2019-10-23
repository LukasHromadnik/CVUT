% Statistical Pattern Recognition Toolbox (STPRtool).
% Version 2.12       12-Sep-2013
%
% Bayesian classification.
%   bayescls         - Bayesian classifier with reject option.
%   bayesdf          - Computes decision boundary of Bayesian classifier.
%   bayeserr         - Computes Bayesian risk for 1D case with Gaussians.
%
% Linear Discriminant function.
%   linclass         - Linear classifier.
%   ekozinec         - Kozinec's algorithm for eps-optimal hyperplane.
%   mperceptron      - Perceptron to train multi-class linear classifier.
%   perceptron       - Perceptron to train binary linear classifier.
%   fld              - Fisher Linear Discriminant. 
%   fldqp            - Fisher Linear Discriminant using QP.
%
% Generalized Anderson's task.
%   andrerr          - Classification error of the Generalized Anderson's task.
%   androrig         - Original method to solve the Anderson's task.
%   eanders          - Epsilon-solution of the Generalized Anderson's task.
%   ganders          - Solves the Generalized Anderson's task.
%   ggradander       - Generalized gradients approach to Gen. Anderson's task.
% 
% Linear feature extraction.
%   linproj          - Linear data projection.
%   lda              - Linear Discriminant Analysis.
%   pca              - Principal Component Analysis.
%   pcarec           - Computes reconstructed vector after PCA projection.
%
% Miscellaneous methods.
%   adaboost         - AdaBoost algorithm.
%   adaclass         - AdaBoost classifier.
%   cerror           - Computes classification error.
%   crossval         - Partions data for cross-validation.
%   knnclass         - k-Nearest Neighbours classifier.
%   knnrule          - Creates K-nearest neighbours classifier.
%   roc              - Computes Receiver Operator Characteristic.
%   sectohms         - Converts seconds to HOUR:MIN:SEC format.
%   weaklearner      - Produces classifier thresholding single feature.
%
% Kernel machines.
%   diagker         - Returns diagonal of kernel matrix.
%   dualcov         - Dual representation of covariance matrix.
%   dualmean        - Computes dual representation of mean vector.
%   kdist           - Computes distance between points in kernel space. 
%   kernel          - Evaluates kernel function.
%   kernelproj      - Kernel projection.    
%   kfd             - Kernel Fisher Discriminant.
%   knorm           - Computes L2-norm in kernel space.
%   kperceptr       - Kernel Perceptron.
%   lin2svm         - Merges linear rule and kernel projection. 
%   minball         - Minimal enclosing ball in kernel feature space.
%   rsrbf           - Reduced Set Method for RBF kernel expansion.   
%   rspoly2         - Reduced Set Method for homegeneous 2nd polynomial kernel.
% 
% Kernel feature extraction.
%   gda             - Generalized Discriminant Analysis.
%   greedykpca      - Greedy kernel PCA.
%   kpca            - Kernel Principal Component Analysis.
%   kpcarec         - Reconstructs image after kernel PCA.
%  
% Optimization methods.
%   gmnp            - Generalized Minimal Norm (GMNP) problem.
%   gnnls           - Generalized Non-negative Least Squares (GNNLS) problem. 
%   gnpp            - Generalized Nearest Point (GNPP) problem. 
%   gridsearch      - Function minimization using grid search.
%   gsmo            - Generalized Sequential Minimal Optimizer.
%   qpbsvm          - Solves QP task required for learning SVM without bias term.
%   qpssvm          - Solves QP task required for StructSVM learning.
%
% Pre-image problem for RBF kernel.
%   rbfpreimg       - Schoelkopf's fixed-point algorithm.
%   rbfpreimg2      - Gradient optimization.
%   rbfpreimg3      - Kwok-Tsang's algorithm.
%
% Support Vector Machines.
%   bsvm2           - Solver for multi-class BSVM with L2-soft margin.
%   evalsvm         - Training and evaluates SVM classifier.
%   mvsvmclass      - Majority voting multi-class SVM classifier.
%   oaasvm          - Multi-class SVM using One-Agains-All decomposition.
%   oaosvm          - Multi-class SVM using One-Against-One decomposition.
%   smo             - Sequential Minimal Optimization for SVM (L1).
%   svm1d           - Linear SVM for 1-dimensional input data.
%   svm2            - Solver for binary SVM with L2 soft margin.
%   svmclass        - Support Vector Machines Classifier. 
%   svmlight        - Interface to SVM^{light} software. 
%   svmquadprog     - SVM trained by Matlab Optimization Toolbox.
%
% Probability distribution functions and estimation.
%   dsamp           - Generates samples from discrete distribution.
%   erfc2           - Normal cumulative distribution function. 
%   gmmsamp         - Generates sample from Gaussian mixture model (GMM).
%   gsamp           - Generates sample from Gaussian distribution.
%   cmeans          - C-means (or K-means) clustering algorithm. 
%   mahalan         - Computes Mahalanobis distance.
%   pdfgauss        - Computes probability for Gaussian distribution.
%   pdfgmm          - Computes probability for Gaussian mixture model.  
%   sigmoid         - Evaluates sigmoid function.
%
%   emgmm           - Expectation-Maximization Algorithm for GMM. 
%   mlcgmm          - ML estimation of GMM from complete data.
%   mlsigmoid       - Fitting a sigmoid function using ML estimation.
%   mmgauss         - Minimax estimation of Gaussian distribution.
%   rsde            - Reduced Set Density Estimator.
%
% Quadratic discriminant function.
%   lin2quad        - Merges linear rule and quadratic mapping.
%   qmap            - Quadratic data mapping.
%   quadclass       - Quadratic classifier.  
%
% Visualization.
%   pandr           - Visualizes solution of the Generalized Anderson's task. 
%   pboundary       - Plots decision boundary of given classifier in 2D.
%   pgauss          - Visualizes set of bivariate Gaussians.
%   pgmm            - Visualizes Gaussian mixture model.
%   pkernelproj     - Plots isolines of kernel projection.
%   plane3          - Plots plane in 3d.
%   pline           - Plots line in 2D.
%   ppatterns       - Plots pattern as points in feature space.
%   psvm            - Plots decision boundary of binary SVM classifier. 
%   showim          - Displays given image(s).  
%   
% Data sets.
%   andersons_task   - (dir) Input for demo on Generalized Anderson's task.
%   binary_separable - (dir) Input for demo on Linear classification.
%   gmm_sample       - (dir) Input for demo on EM algorithm for GMM.
%   iris_data        - (dir) Fisher's Iris data set.
%   mm_sample        - (dir) Input for demo on Minimax Algorithm.
%   multi_separable  - (dir) Linearly separable multi-class data.
%   ocr_numerals     - (dir) Examples of hand-written numerals.
%   riply_data       - (dir) Riply's data set.
%   svm_sample       - (dir) Input for demo on SVM.
%
%   c2s              - Converts cell to structure array.
%   createdata       - Interactive data generator. 
%   gencircledata    - Generates data on circle corrupted by Gaussian noise. 
%   genlsdata        - Generates linearly separable binary data.
%   mergesets        - Merges data sets to one labeled data file.
%   savestruct       - Saves fields of given structure to file.
%   usps2mat         - Converts USPS database to Matlab data file (MAT). 
%
% Demos.
%   image_denoising - (dir) Image denoising using kernel PCA.
%   ocr             - (dir) Optical Character Recognition.  
%
%   demo_anderson   - Generalized Anderson's task.
%   demo_emgmm      - Expectation-Maximization algorithm for GMM.
%   demo_kpcadenois - Idea of image denoising based on Kernel PCA.
%   demo_linclass   - Algorithms learning linear classifiers.
%   demo_mmgauss    - Minimax estimation of Gaussian distribution.
%   demo_ocr        - Run OCR demo.
%   demo_pcacomp    - Image compression using PCA.
%   demo_svm        - Support Vector Machines.
%   demo_svmpout    - Fitting a posteriori probability to SVM output.
%
%
% compilemex - Compiles all MEX files of the STPRtool.
% stprpath   - Sets path to the STPRtool.
% 

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2005, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 09-sep-2007
% 16-jul-2007, VF
% 02-jul-2007, VF
% 17-jun-2007, VF
% 26-mar-2007, VF
% 20-nov-2006, VF
% 19-sep-2006, VF
% 18-sep-2006, VF
% 09-sep-2005, VF
% 06-jun-2005, VF
% 24-jan-2005, VF
% 22-dec-2004, VF
% 14-dec-2004, VF
% 08-oct-2004, VF
% 27-aug-2004, VF
% 15-jun-2004, VF
% 11-jun-2004, VF
% 20-sep-2003, VF
