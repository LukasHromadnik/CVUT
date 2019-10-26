library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity debounce is
	port (
		clk     : in std_logic;
		tl_in   : in std_logic;
		tl_out  : out std_logic
	);
end debounce;

architecture arch of debounce is

signal cnt      : std_logic_vector(19 downto 0);	-- 20 ms	=> F4240 for 50Mhz
signal tl_prev  : std_logic;

begin

	p_deb:process(clk)
	begin

		if rising_edge(clk) then
			if (cnt = x"00001") then
				tl_out <= tl_prev;
			end if;
			
			if (cnt /= x"00000") then
				cnt <= cnt - 1;
			end if;

			if (tl_in /= tl_prev) then
				tl_prev <= tl_in;
				cnt <= x"F4240";
			end if;
			
		end if;
	end process;


end arch;