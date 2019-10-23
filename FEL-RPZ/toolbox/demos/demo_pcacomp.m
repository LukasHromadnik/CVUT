% DEMO_PCACOMP Demo on image compression using PCA
%
% Description:
%  The PCA model is trained to model rectangular non-overlapping 
%  patches of a specified input image. The compression phase 
%  involves representation of the image patches in the lower 
%  dimesional space trained by the PCA. The decompression consists 
%  of reconstruction of the image patches from their low dimensional 
%  representation.
%
% Note: This demo requires Images toolbox.
%
% See also 
%  PCA, LINPROJ, PCAREC.
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>

% Modifications:
% 06-jun-2004, VF

help demo_pcacomp;

% Setting
%==========================================
img_file = 'lena.jpg';
patch_size = 16;

fprintf('Input dimension: %d\n', patch_size^2);
new_dim=input('Output dimesion:');
if isempty(new_dim), new_dim = 20; end

% load image
fprintf('Image: %s\n', img_file);
img = im2double( imread( img_file ));
if isrgb(img), img = rgb2grey(img); end
fprintf('Image size: [%d x %d]\n', size(img));

% image -> patches stored as column vectors
X = im2col( img, patch_size*[1 1], 'distinct');

% train PCA
fprintf('Training PCA...');
pca_model = pca(X,new_dim );
fprintf('done\n');

% reconstruction
fprintf('Reconstructing image patches...');
Y = pcarec( X, pca_model );
fprintf('done\n');

% plot reconstruction error w.r.t. to dimension
figure; hold on; 
title('Reconstruction error'); 
xlabel('output dimesion'); ylabel('MsErr');
plot(pca_model.MsErr);
plot(pca_model.new_dim, pca_model.mse, 'or');

% patches -> image
compressed_img = col2im(Y,patch_size*[1 1],size(img),'distinct');

% display original and compresed image
figure; imshow(img);
hold on; title('Original image');

% display original and compresed image
figure; imshow(img);
hold on; title('Splitting to patches');
[h,w]=size(img);
for x=1:16:w, plot([x x],[1 h],'b'); end
for y=1:16:w, plot([1 w],[y y],'b'); end

figure; imshow(compressed_img);
hold on; title('Compressed image');

% display statistics
[dim,num_data]=size(X);
new_dim=pca_model.new_dim;
cr = ((new_dim*num_data)+(new_dim*dim))/(dim*num_data);

fprintf('new_dim=%d\n',new_dim);
fprintf('Compression ratio (compressed_size/original_size)= %f%%\n',cr*100);
fprintf('MsErr=%f\n', pca_model.mse);

%EOF