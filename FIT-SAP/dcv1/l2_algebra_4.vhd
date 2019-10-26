library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- definice vstup� a v�stup� - vstupy a, b, c, v�stupy f
entity algebra is
    Port ( d : in std_logic;
	   c : in std_logic;
           b : in std_logic;
           a : in std_logic;
           f : out std_logic);
end algebra;

-- vlastn� popis chov�n� obvodu -&gt; v�po�et funkce f
architecture Behavioral of algebra is
begin
  -- ve v�razech pou�ivejte n�sleduj�c� oper�tory:
  -- ----------------------------------
  -- | oper�tor | p��klad pou�it�     |
  -- ----------------------------------
  -- |    and   |	a and b and c       |
  -- |    or    |	a or b or (c and d) |
  -- |    not   | not a               |
  -- ----------------------------------
  -- pou��vejte z�vorky!
  -- p��klad:
  --   zad�n�:                            f(a, b, c) = a * #b * #c + a * b
  --   reprezentace na n�sleduj�c� ��dce: f &lt;= (a and (not b) and (not c)) or (a and b)
  -- funkce z n�vodu m� tvar:
  f <= ((not c) and (not b) and (not d)) or ((not c) and (not d) and a) or (c and b and a) or (c and b and (not d)) or (c and (not a) and (not b)) or (a and (not c) and (not b));
end Behavioral;