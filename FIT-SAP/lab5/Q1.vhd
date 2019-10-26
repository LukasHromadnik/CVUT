-- knihovna typů pro práci
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- definice vstupů a výstupů - vstupy a, b, c, výstupy f
entity Q1 is
Port ( x : in std_logic;
	q2 : in std_logic;
	q1 : in std_logic;
	q0 : in std_logic;
	f : out std_logic);
end Q1;

-- vlastní popis chování obvodu -> výpočet funkce f
architecture Behavioral of Q1 is
begin
-- ve výrazech použivejte následující operátory:
-- ----------------------------------
-- | operátor | příklad použití     |
-- ----------------------------------
-- |    and   |	a and b and c       |
-- |    or    |	a or b or (c and d) |
-- |    not   | not a               |
-- ----------------------------------
-- používejte závorky!
-- příklad:
--   zadání:                            f(a, b, c) = a * #b * #c + a * b
--   reprezentace na následující řádce: f <= (a and (not b) and (not c)) or (a and b)
-- funkce z návodu má tvar:
f <= (q1 and (not x) and (not q0)) or (q0 and x and (not q1));
end Behavioral;