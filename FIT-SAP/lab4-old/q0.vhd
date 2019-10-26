-- knihovna typ� pro pr�ci
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- definice vstup� a v�stup� - vstupy a, b, c, v�stupy f
entity q0 is
    Port ( d : in std_logic;
	   c : in std_logic;
           b : in std_logic;
           a : in std_logic;
           f : out std_logic);
end q0;

-- vlastn� popis chov�n� obvodu -> v�po�et funkce f
architecture Behavioral of q0 is
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
  f <= (a and (not d)) or (d and (not a));
end Behavioral;
