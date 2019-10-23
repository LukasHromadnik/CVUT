function output = compute_feature(letter_char, Alphabet, images, labels)
    letter = upper(letter_char);
    letterPosition = strfind(Alphabet, letter);
    ims = images(:, :, labels == letterPosition);
    x = [];
    for i = 1:size(ims, 3)
        im = ims(:,:,i);
        result = sum(sum(im(:,1:size(im)/2))) - sum(sum(im(:,(size(im)/2 + 1):end)));
        x = [x; result];
    end
    output = x;
end
