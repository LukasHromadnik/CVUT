library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- definice vstupu a vystupu - vstupy a, b, c, d, vystupy f
entity sega is
Port (
    d : in std_logic;
    c : in std_logic;
    b : in std_logic;
    a : in std_logic;
    f : out std_logic
);
end sega;

-- vlastni popis chovani obvodu > vypocet funkce f
architecture Behavioral of sega is
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
f <= (d and (not b)) or (a and c) or (b and (not d)) or (b and (not a) and (not c)) or ((not c) and (not d) and (not b));
end Behavioral;