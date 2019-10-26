library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
  
-- pr�zdn� entita zapouzd�uj�c� testovac� sadu
ENTITY scit4_test IS
END scit4_test;
 
-- popis chov�n� testovac� sady
-- hlavi�ka ve formatu: ARCHITECTURE &lt;libovolny_nazev&gt; OF &lt;nazev_entity&gt; IS
ARCHITECTURE behavior OF scit4_test IS 
 
    -- definice vstup� a v�stup� testovan� komponenty
    -- vstupy:  CIN, A0..A3, B0..B3 
    -- v�stupy: S0..S3, COUT
    -- POZOR!
    -- N�ZEV KOMPONENTY (COMPONENT &lt;nazev&gt;) MUS� B�T STEJN� JAKO N�ZEV TESTOVAN�HO SCH�MATU
    -- V TOMTO P��PAD�: COMPONENT scit4
    -- VSTUPY A V�STUPY KOMPONENTY (COMPONENT scit4) MUS� B�T STEJN� JAKO POPISY VSTUP� A V�STUP� VE SCH�MATU

    COMPONENT scit4
    PORT(
         A    : IN  std_logic;
         B    : IN  std_logic;
         C    : IN  std_logic;
         D    : IN  std_logic;
         F    : OUT  std_logic
        );
    END COMPONENT;
    
   -- vstupn� sign�ly ovl�dan� testovac� sadou
   signal A    : std_logic := '0';
   signal B    : std_logic := '0'; 
   signal C    : std_logic := '0';
   signal D    : std_logic := '0';

    -- v�stupn� sign�ly ovl�dan� testovac� sadou
   signal F    : std_logic;
 
BEGIN
 
   -- mapov�n� vstup� a v�stup� testovan� komponenty na sign�ly ovl�dan� testovac� sadou
   uut: scit4 PORT MAP (
      A => A,
      B => B,
      C => C,
      D => D,
      F => F
   );
 
   -- simula�n� proces realizuj�c� chov�n� testovac� sady
   stim_proc: process
   begin      
      -- po��te�n� nastaven� vstup�
      A    <= '0';
      B    <= '0'; 
      C    <= '0';
      D    <= '0';   
      wait for 100 ns;
   

      -------------------------------------------------
      -- testov�n� n�kter�ch mo�n�ch kombinac�
      -------------------------------------------------

      -------------------------------------------------
      -- P��KLAD 1:
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

      wait for 20 ns; -- udr�en� hodnot po dobu 20 ns
      -- vracen� po��te�n�ch hodnot
      A    <= '0';
      B    <= '0';
		C    <= '0';
		D    <= '0';
      -- konec p��kladu 1


      -------------------------------------------------
      -- P��KLAD 2:
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

      wait for 20 ns; -- udr�en� hodnot po dobu 20 ns
      -- vracen� po��te�n�ch hodnot
      A    <= '0';
      B    <= '0';
      C    <= '0';
      D    <= '0';
      -- konec p��kladu 2

      -- Dal�� testy:
      -- .... dopl�te
      
      -- KONEC
      
      wait;   -- nekone�n� �ekac� smy�ka
   end process;

END;