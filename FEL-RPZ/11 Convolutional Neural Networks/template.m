% Init MatConvNet framework
MCNPath = 'matconvnet-1.0-beta23';
run(fullfile(MCNPath, 'matlab/vl_setupnn'))  

% load data
load imdb

% normalise the data
imdb.images.data = imdb.images.data / 255;

% training and validation sets
imdb.images.set = [1 * ones(1, 50000), 2 * ones(1, 10000)];
%imdb.images.set = [1 * ones(1, 10000), 2 * ones(1, 1000)];

delete expDir/*
clear net;

% TRAINED
net.layers = {};
net.layers{end+1} = struct('type', 'conv', ...
			   'weights', {{1e-2 * randn(5, 5, 1, 20, 'single'), zeros(1, 20, 'single')}}, ...
			   'stride', 1, ...
			   'pad', 0);
net.layers{end+1} = struct('type', 'pool', 'method', 'max', 'pool', [2 2], 'stride', 2, 'pad', 0);
net.layers{end+1} = struct('type', 'conv', ...
			   'weights', {{1e-2 * randn(5, 5, 20, 50, 'single'), zeros(1, 50, 'single')}}, ...
			   'stride', 1, ...
			   'pad', 0);
net.layers{end+1} = struct('type', 'pool', 'method', 'max', 'pool', [2 2], 'stride', 2, 'pad', 0);
net.layers{end+1} = struct('type', 'conv', ...
			   'weights', {{1e-2 * randn(4, 4, 50, 500, 'single'), zeros(1, 500, 'single')}}, ...
			   'stride', 1, ...
			   'pad', 0);
net.layers{end+1} = struct('type', 'pool', 'method', 'max', 'pool', [2 2], 'stride', 2, 'pad', 1);
net.layers{end+1} = struct('type', 'relu');
net.layers{end+1} = struct('type', 'conv', ...
			   'weights', {{1e-2 * randn(1, 1, 500, 10, 'single'), zeros(1, 10, 'single')}}, ...
			   'stride', 1, ...
			   'pad', 0);
net.layers{end+1} = struct('type', 'softmaxloss');

net = vl_simplenn_tidy(net);
vl_simplenn_display(net)

[net, info] = cnn_train(net, imdb, @getSimpleNNBatch, 'batchSize', 1000, 'numEpochs', 20, 'expDir', 'expDir');   %'plotStatistics', false); 

% dropout layer
%net.layers{end+1} = struct('type', 'dropout', 'rate', 0.5) ;

% save your best network
net.layers{end}.type = 'softmax';
save('my_cnn.mat', 'net');
