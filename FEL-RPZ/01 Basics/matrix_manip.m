function output = matrix_manip(A,B)
% output = matrix_manip(A,B)
%
%   Perform example matrix manipulations.
%
%   Parameters:
%       A - matrix, arbitrary shape
%       B - matrix, <2 x n>
%
%   Return:
%       output.A_transpose
%       output.A_3rd_col
%       output.A_slice
%       output.A_gr_inc
%       output.C
%       output.A_weighted_col_sum
%       output.D
%       output.D_select
   
    % 1. Find the transpose of the matrix A:
    output.A_transpose = A';

    % 2. Select the third column of the matrix A:
    output.A_3rd_col = A(:,3);

    % 3. Select last two rows and last three columns of the matrix A and return the matrix in output.A_slice. 
    output.A_slice = A(end - 1:end, end - 2:end);

    % 4.Find all positions in A greater then 3 and increment them by 1 and add a column of ones to the matrix. Save the result to matrix A_gr_inc:
    my_matrix = A + (A > 3);
    my_matrix(:,end + 1) = 1;
    output.A_gr_inc = my_matrix;

    % 5. Create matrix C such that Ci,j=∑nk=1A_gr_inci,k⋅A_gr_incTk,j and store it in output.C. 
    
    output.C = my_matrix * my_matrix';

    % 6. Compute ∑nc=1c⋅∑mr=1A_gr_incr,c:
    my_matrix_size = size(my_matrix);
    output.A_weighted_col_sum = sum((1:my_matrix_size(2)).*sum(my_matrix));

    % 7. Subtract a vector (4,6)T from all columns of matrix B. Save the result to matrix output.D.
    size_b = size(B);
    output.D = B - repmat([4, 6]', 1, size_b(2));

    % 8. Select all vectors in the matrix D, which have greater euclidean distance than the average euclidean distance.
    sizes = sqrt(sum(output.D.^2));
    mean_size = mean(sizes);
    toRemove = sizes < mean_size;
    output.D_select = output.D(:,toRemove < 1);

end
