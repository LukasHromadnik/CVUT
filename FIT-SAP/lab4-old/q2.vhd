-- knihovna typù pro práci
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- definice vstupù a výstupù - vstupy a, b, c, výstupy f
entity q2 is
    Port ( d : in std_logic;
			  c : in std_logic;
           b : in std_logic;
           a : in std_logic;
           f : out std_logic);
end q2;

-- vlastní popis chování obvodu -> výpoèet funkce f
architecture Behavioral of q2 is
begin
  -- ve výrazech použivejte následující operátory:
  -- ----------------------------------
  -- | operátor | pøíklad použití     |
  -- ----------------------------------
  -- |    and   |	a and b and c       |
  -- |    or    |	a or b or (c and d) |
  -- |    not   | not a               |
  -- ----------------------------------
  -- používejte závorky!
  -- pøíklad:
  --   zadání:                            f(a, b, c) = a * #b * #c + a * b
  --   reprezentace na následující øádce: f <= (a and (not b) and (not c)) or (a and b)
  -- funkce z návodu má tvar:
  f <= (c and (not b)) or (c and (not a)) or (c and (not d)) or (a and b and d and (not c));
end Behavioral;
