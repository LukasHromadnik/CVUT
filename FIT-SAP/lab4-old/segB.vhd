-- knihovna typù pro práci
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- definice vstupù a výstupù - vstupy a, b, c, výstupy f
entity segB is
    Port ( d : in std_logic;
	   c : in std_logic;
           b : in std_logic;
           a : in std_logic;
           f : out std_logic);
end segB;

-- vlastní popis chování obvodu -> výpoèet funkce f
architecture Behavioral of segB is
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
  f <= (c and (not a) and d) or (a and (not b) and (not d) and c) or (d and a and b) or (c and b and (not a));
end Behavioral;