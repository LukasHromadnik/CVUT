library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- popis vstup˘ a v˝stup˘ funkce - vstupy a, b, c, d, v˝stup f
entity truth_table is
    Port ( d : in std_logic;
           c : in std_logic;
           b : in std_logic;
           a : in std_logic;
           f : out std_logic);
end truth_table;

-- chov·nÌ funkce - jako pamÏù ROM, a, b, c, d je adresa, f jsou data
architecture Behavioral of truth_table is

-- definice typu PRAVDIVOSTNÕ TABULKA - obsahuje 16 v˝stup˘ (2^4 = 16) typu standardnÌ logika - nuly a jedniËky
  type truth_table is array (0 to 15) of std_logic;

-- definice NAäÕ FUNKCE f(d, c, b, a) pravdivostnÌ tabulkou
  constant our_function : truth_table := truth_table'(
    -- ------------------------------------------------------------------------------
    -- |  funËknÌ hodnota f(d, c, b, a) |  stavov˝ index s  |  d  |  c  |  b  |  a  | 
    -- ------------------------------------------------------------------------------
                     '1',            -- |           0       |  0  |  0  |  0  |  0  |
                     '1',            -- |           1       |  0  |  0  |  0  |  1  |
                     '0',            -- |           2       |  0  |  0  |  1  |  0  |
                     '1',            -- |           3       |  0  |  0  |  1  |  1  |
                     '1',            -- |           4       |  0  |  1  |  0  |  0  |
                     '0',            -- |           5       |  0  |  1  |  0  |  1  |
                     '1',            -- |           6       |  0  |  1  |  1  |  0  |
                     '1',            -- |           7       |  0  |  1  |  1  |  1  |
                     '0',            -- |           8       |  1  |  0  |  0  |  0  |
                     '1',            -- |           9       |  1  |  0  |  0  |  1  |
                     '0',            -- |          10       |  1  |  0  |  1  |  0  |
                     '0',            -- |          11       |  1  |  0  |  1  |  1  |
                     '1',            -- |          12       |  1  |  1  |  0  |  0  |
                     '0',            -- |          13       |  1  |  1  |  0  |  1  |
                     '0',            -- |          14       |  1  |  1  |  1  |  0  |
                     '1'             -- |          15       |  1  |  1  |  1  |  1  |
    -- ------------------------------------------------------------------------------
  );

	signal address : std_logic_vector(3 downto 0);
begin
	address(3) <= d;
	address(2) <= c;
	address(1) <= b;
	address(0) <= a;

	f <= our_function(conv_integer(address(3 downto 0)));

end Behavioral;