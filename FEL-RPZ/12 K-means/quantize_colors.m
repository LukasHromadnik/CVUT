function im_q = quantize_colors(im, k)
% im_q = quantize_colors(im, k)
%
% Image color quantization using the k-means clustering. The pixel colors
% are at first clustered into k clusters. Color of each pixel is then set
% to the mean color of the cluster to which it belongs to.
%
% Input:
%   im        .. Image whose pixel colors are to be quantized.
%
%   k         .. Required number of quantized colors.
%
% Output:
%   im_q      .. Image with quantized colors.


% Convert the image from RGB to L*a*b* color space
cform = makecform('srgb2lab');
im = applycform(im, cform);

height = size(im, 1);
width = size(im, 2);

number_of_pixels = 1000;

indices = randsample(height * width, number_of_pixels);
[I, J] = ind2sub([height width], indices);

pixels = zeros(3, number_of_pixels);

for i = 1:number_of_pixels
    result = im(I(i), J(i), :);
    pixels(:, i) = result(:);
end

[~, means, ~] = k_means(pixels, k, Inf);

im_q = zeros(height, width, 3, 'uint8');

for i = 1:(height * width)
    [row, col] = ind2sub([height width], i);
    pixel = double(reshape(im(row, col, :), [3 1]));
    [~, min_idx] = min(sum((pixel - means).^2));
    im_q(row, col, :) = uint8(means(:, min_idx));
end

% Convert the image from L*a*b* back to RGB
cform = makecform('lab2srgb');
im_q = applycform(im_q, cform);
