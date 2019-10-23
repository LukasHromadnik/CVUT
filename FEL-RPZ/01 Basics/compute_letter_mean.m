function letter_mean = compute_letter_mean(letter_char, Alphabet, images, labels)
% img = compute_letter_mean(letter_char, Alphabet, images, labels)
%
% Compute mean image for a letter.
%
% Parameters:
%   letter_char         character, e.g. 'm'
%   Alphabet            list of all characters present in images, e.g. 'abcdefgh'
%   images              images of letters, matrix of size (height x width x number of images)
%   labels              image labels, vector of size <number of images>; <label> is index to <Alphabet>
%
% Return:
%   letter_mean         mean of all images of the <letter_char>, uint8 type

    letter = upper(letter_char);
    letterPosition = strfind(Alphabet, letter);
    ims = images(:, :, labels == letterPosition);
    letter_mean = uint8(mean(ims, 3));
    
end
