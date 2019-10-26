library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- definice vstupù a výstupù - vstupy a, b, c, výstupy f
entity algebra is
    Port ( d : in std_logic;
	   c : in std_logic;
           b : in std_logic;
           a : in std_logic;
           f : out std_logic);
end algebra;

-- vlastní popis chování obvodu -&gt; výpoèet funkce f
architecture Behavioral of algebra is
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
  --   reprezentace na následující øádce: f &lt;= (a and (not b) and (not c)) or (a and b)
  -- funkce z návodu má tvar:
  f <= ((not c) and (not b) and (not d)) or ((not c) and (not d) and a) or (c and b and a) or (c and b and (not d)) or (c and (not a) and (not b)) or (a and (not c) and (not b));
end Behavioral;