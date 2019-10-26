-- knihovny
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- definice vstupu a vystupu
entity segment7 is
Port (
  d : in std_logic;
  c : in std_logic;
  b : in std_logic;
  a : in std_logic;
  f1 : out std_logic;
  f2 : out std_logic;
  f3 : out std_logic;
  f4 : out std_logic;
  f5 : out std_logic;
  f6 : out std_logic;
  f7 : out std_logic
);
end segment7;

architecture Behavioral of segment7 is
begin
  
  -- segment A
  f1 <= (a and (not b) and (not c) and (not d)) or (c and (not a) and (not b) and (not d)) or (a and d and c and (not b)) or (b and d and a and (not c));

  -- segment B
  f2 <= (c and (not a) and d) or (a and (not b) and (not d) and c) or (d and a and b) or (c and b and (not a));

  -- segment C
  f3 <= (b and (not c) and (not d) and (not a)) or (b and d and c) or (d and c and (not a));

  -- segment D
  f4 <= (a and (not b) and (not c) and (not d)) or (c and (not a) and (not b) and (not d)) or (b and c and a) or ((not a) and b and d and (not c));

  -- segment E
  f5 <= (a and (not d)) or (c and (not d) and (not b)) or (a and (not c) and (not b));

  -- segment F
  f6 <= (a and (not c) and (not d)) or (b and (not c) and (not d)) or (a and (not d) and b) or (a and c and d and (not b));

  -- segment G
  f7 <= ((not c) and (not d) and (not b)) or (a and b and c and (not d)) or (c and d and (not a) and (not b));

end Behavioral;