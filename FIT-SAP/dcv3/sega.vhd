
<!-- saved from url=(0071)https://edux.fit.cvut.cz/courses/BI-SAP/_media/labs/02/l2_algebra_4.vhd -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1250"><style type="text/css"></style></head><body><pre style="word-wrap: break-word; white-space: pre-wrap;">-- knihovna typů pro práci
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- definice vstupů a výstupů - vstupy a, b, c, výstupy f
entity algebra is
    Port ( d : in std_logic;
	   c : in std_logic;
           b : in std_logic;
           a : in std_logic;
           f : out std_logic);
end algebra;

-- vlastní popis chování obvodu -&gt; výpočet funkce f
architecture Behavioral of algebra is
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
  --   reprezentace na následující řádce: f &lt;= (a and (not b) and (not c)) or (a and b)
  -- funkce z návodu má tvar:
  f &lt;= ((not c) and (not b) and a) or ((not c) and b and (not a)) or (c and (not b) and (not a)) or (c and b and (not a));
end Behavioral;
</pre></body></html>