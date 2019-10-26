library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- definice vstupu a vystupu - vstupy a, b, c, d, vystupy f
entity segf is
Port (
    d : in std_logic;
    c : in std_logic;
    b : in std_logic;
    a : in std_logic;
    f : out std_logic
);
end segf;

-- vlastni popis chovani obvodu > vypocet funkce f
architecture Behavioral of segf is
begin
-- ve ve vyrazech pouzivejte nasledujici operatory:
-- ----------------------------------
-- | operator | priklad pouzitu     |
-- ----------------------------------
-- |    and   |	a and b and c       |
-- |    or    |	a or b or (c and d) |
-- |    not   | not a               |
-- ----------------------------------
-- pouzivejte zavorky!
-- priklad:
--   zadani:                            f(a, b, c) = a * #b * #c + a * b
--   reprezentace na následující øádce: f <= (a and (not b) and (not c)) or (a and b)
-- funkce z navodu ma tvar:
f <= (d and b and (not c)) or (c and (not b) and (not d)) or ((not a) and (not b) and (not d)) or (d and b and (not a)) or (c and b and (not a)) or (b and c and d);
end Behavioral;