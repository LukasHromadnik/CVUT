-- knihovna typ� pro pr�ci
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- definice vstup� a v�stup� - vstupy a, b, c, v�stupy f
entity Q2 is
    Port ( q0 : in std_logic;
	        q1 : in std_logic;
           q2 : in std_logic;
           en : in std_logic;
           f : out std_logic);
end Q2;

-- vlastn� popis chov�n� obvodu -> v�po�et funkce f
architecture Behavioral of Q2 is
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
  --   reprezentace na n�sleduj�c� ��dce: f <= (a and (not b) and (not c)) or (a and b)
  -- funkce z n�vodu m� tvar:
  f <= ((q2 and (not q1)) or (q2 and (not q0)) or (q2 and (not en)) or (q0 and q1 and en and (not q2)));
end Behavioral;
