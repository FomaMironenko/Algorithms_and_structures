% A solution of linear optimization problem
% in canonical form:
% 
% x \in R^n
% c \in R^n
% A \in R^(m * n)
% b \in R^m
%
% c' * x -> inf
% A * x = b
% x >= 0



% A = [ 2, 1,  -3,  0,  0;
%      -3, 0,  -2,  0,  1;
%       1, 0,   1,  1,  0];
%   
% b = [10; 4; 7];
% 
% c = [-2; 1; -3; -1; 0];


% A = [ 2, -1,  0, -2,  1,  0;
%       3,  2,  1, -3,  0,  0;
%      -1,  3,  0,  4,  0,  1];
%   
% b = [16; 18; 24];
% 
% c = [-2; -3; 0; 1; 0; 0];


A = [10,  20,  25,  40;
     100, 100, 100, 100];

b = [30; 100];

c = [10; 30; 40; 60];


plan = solve(A, b, c);
cprintf('green', "\n\n\nanswer\n\n");
plan %#ok
targ = c' * plan %#ok




function sol = solve(A, b, c)
    m = size(A, 1); n = size(A, 2);
    assert(ismatrix(A));
    assert(rank(A) == size(A, 1));
    
    A(b < 0, :) = -A(b < 0, :);
    b(b < 0) = -b(b < 0);
    
    A1 = zeros(m, n + m);
    A1(:, 1:n) = A;
    A1(:, n+1:n+m) = eye(m);
    c1 = zeros(n + m, 1);
    c1(1:n) = c;
    c1(n+1:end) = 1000;
    Npl = (n+1):(n+m);
    x0  = zeros(n+m, 1);
    x0(n+1:n+m) = b;
    
    res = simplex(A1, b, c1, x0, Npl);
    assert(all(abs(res(n+1:n+m)) < 1e-10));
    sol = res(1:n);
end


function sol = simplex(A, b, c, x, Npl)
    m = size(A, 1); n = size(A, 2);
    
    assert(ismatrix(A));
    assert(all(x >= 0));
    assert(all(abs(A*x - b) < 1e-10));
    assert(det(A(:, Npl)) ~= 0);
    assert(numel(Npl) == m);
    
    cprintf('blue', "\n\niteration\n")
    
    B = inv(A(:, Npl));
    u = c(Npl)' * B; %#ok
    
    Nmi = setdiff(1:n, Npl);
    delta = u * A(:, Nmi) - c(Nmi)';
    
    Npl %#ok
    xk = x(Npl);
    xk %#ok
    B %#ok
    
    if all(delta <= 0)
        Nmi %#ok
        delta %#ok
        sol = x;
        return;
    end
    
    [~, imax] = max(delta);
    jk = Nmi(imax);
    delt = delta(imax);
    delt %#ok
    z  = B * A(:, jk) %#ok
    if all(z <= 0)
        sol = [];
        return;
    end
    
    mask = (z > 0);
    assert(any(mask));
    i0 = find(mask, 1);
    tk = x(Npl(i0)) / z(i0);
    sk = i0;
    inds = find(mask);
    for ii = 1:numel(inds)
        i = inds(ii);
        if x(Npl(i)) / z(i) < tk
            tk = x(Npl(i)) / z(i);
            sk = i;
        end
    end
    
    N1 = Npl;
    N1(sk) = jk;
    
    x1 = x;
    x1(Npl) = x(Npl) - tk * z;
    x1(Nmi) = 0;
    x1(jk)  = tk;
    assert(~any(x1(N1) <= 0), "degenerate problem");
    
    
    targ = c(Npl)' * x(Npl);
    targ %#ok
    u %#ok
    
    sol = simplex(A, b, c, x1, N1);
end

