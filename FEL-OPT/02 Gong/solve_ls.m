function output = solve_ls(A, b)
    [Q, R] = qr(A, 0);
    
    output = my_inversion(R) * Q' * b;
end
