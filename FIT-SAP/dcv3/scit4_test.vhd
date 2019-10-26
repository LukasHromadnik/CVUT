library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
  
-- prázdná entita zapouzdøující testovací sadu
ENTITY scit4_test IS
END scit4_test;
 
-- popis chování testovací sady
-- hlavièka ve formatu: ARCHITECTURE &lt;libovolny_nazev&gt; OF &lt;nazev_entity&gt; IS
ARCHITECTURE behavior OF scit4_test IS 
 
    -- definice vstupù a výstupù testované komponenty
    -- vstupy:  CIN, A0..A3, B0..B3 
    -- výstupy: S0..S3, COUT
    -- POZOR!
    -- NÁZEV KOMPONENTY (COMPONENT &lt;nazev&gt;) MUSÍ BÝT STEJNÝ JAKO NÁZEV TESTOVANÉHO SCHÉMATU
    -- V TOMTO PØÍPADÌ: COMPONENT scit4
    -- VSTUPY A VÝSTUPY KOMPONENTY (COMPONENT scit4) MUSÍ BÝT STEJNÉ JAKO POPISY VSTUPÙ A VÝSTUPÙ VE SCHÉMATU

    COMPONENT scit4
    PORT(
         A    : IN  std_logic;
         B    : IN  std_logic;
         C    : IN  std_logic;
         D    : IN  std_logic;
         F    : OUT  std_logic
        );
    END COMPONENT;
    
   -- vstupní signály ovládané testovací sadou
   signal A    : std_logic := '0';
   signal B    : std_logic := '0'; 
   signal C    : std_logic := '0';
   signal D    : std_logic := '0';

    -- výstupní signály ovládané testovací sadou
   signal F    : std_logic;
 
BEGIN
 
   -- mapování vstupù a výstupù testované komponenty na signály ovládané testovací sadou
   uut: scit4 PORT MAP (
      A => A,
      B => B,
      C => C,
      D => D,
      F => F
   );
 
   -- simulaèní proces realizující chování testovací sady
   stim_proc: process
   begin      
      -- poèáteèní nastavení vstupù
      A    <= '0';
      B    <= '0'; 
      C    <= '0';
      D    <= '0';   
      wait for 100 ns;
   

      -------------------------------------------------
      -- testování nìkterých možných kombinací
      -------------------------------------------------

      -------------------------------------------------
      -- PØÍKLAD 1:
      -- CIN=0, A+B = 0001 + 0001 = 0010 -&gt; (MSB..LSB)
      -------------------------------------------------
      A    <= '1';
      B    <= '1';
		C    <= '1';
		D    <= '1';
      wait for 5 ns;
      assert F='1'
      report "dobre";
      --severity ERROR;

      wait for 20 ns; -- udržení hodnot po dobu 20 ns
      -- vracení poèáteèních hodnot
      A    <= '0';
      B    <= '0';
		C    <= '0';
		D    <= '0';
      -- konec pøíkladu 1


      -------------------------------------------------
      -- PØÍKLAD 2:
      -- CIN=1, A+B = 0001 + 0001 = 0011 -&gt; (MSB..LSB)
      -------------------------------------------------
      A    <= '1';
      B    <= '1';
      C    <= '1';
		D    <= '0';
      wait for 5 ns;
      assert F='0'
      report "chyba"
      severity ERROR;

      wait for 20 ns; -- udržení hodnot po dobu 20 ns
      -- vracení poèáteèních hodnot
      A    <= '0';
      B    <= '0';
      C    <= '0';
      D    <= '0';
      -- konec pøíkladu 2

      -- Další testy:
      -- .... doplòte
      
      -- KONEC
      
      wait;   -- nekoneèná èekací smyèka
   end process;

END;