library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
  
-- pr�zdn� entita zapouzd�uj�c� testovac� sadu
ENTITY l2_truth_table_4_test IS
END l2_truth_table_4_test;
 
-- popis chov�n� testovac� sady
-- hlavi�ka ve formatu: ARCHITECTURE &lt;libovolny_nazev&gt; OF &lt;nazev_entity&gt; IS
ARCHITECTURE behavior OF l2_truth_table_4_test IS 
 
   -- definice vstup� a v�stup� testovan� komponenty
   -- vstupy:  a, b, c, d
   -- v�stupy: f
   -- POZOR!
   -- N�ZEV KOMPONENTY (COMPONENT &lt;nazev&gt;) MUS� B�T STEJN� JAKO N�ZEV TESTOVAN� ENTITY
   -- V TOMTO P��PAD�: COMPONENT truth_table
   -- VSTUPY A V�STUPY KOMPONENTY (COMPONENT truth_table) MUS� B�T STEJN� JAKO POPISY VSTUP� A V�STUP� TESTOVAN� ENTITY
   COMPONENT truth_table
   PORT(
     d : IN  std_logic;
     c : IN  std_logic;
     b : IN  std_logic;
     a : IN  std_logic;
     f : OUT  std_logic
   );
   END COMPONENT;
    
   -- vstupn� sign�ly ovl�dan� testovac� sadou
   signal d : std_logic := '0';
   signal c : std_logic := '0';
   signal b : std_logic := '0';
   signal a : std_logic := '0';

   -- v�stupn� sign�ly ovl�dan� testovac� sadou
   signal f : std_logic;
 
BEGIN
 
   -- mapov�n� vstup� a v�stup� testovan� komponenty na sign�ly ovl�dan� testovac� sadou
   uut: truth_table PORT MAP (
      d => d,
      c => c,
      b => b,
      a => a,
      f => f
   );
 
   -- simula�n� proces realizuj�c� chov�n� testovac� sady
   stim_proc: process
      -- deklarace prom�nn�ch pro pr�ci s for-cykly
      variable i, j, k, l  :  integer;
   begin		
      -- po��te�n� nastaven� vstup�
      d   <=   '0';
      c   <=   '0';
      b   <=   '0';
      a   <=   '0';
		
      -- testov�n� v�ech mo�n�ch kombinac�
      -- 4 vstupy =&gt; 4 for-cykly =&gt; 2^4 mo�n�ch vstupn�ch kombinac�
      for i in 0 to 1 loop	
         d   <=   not(d);
         for j in 0 to 1 loop
            c   <=   not(c);
            for k in 0 to 1 loop
               b   <=   not(b);
               for l in 0 to 1 loop
                  a   <=   not(a);
                  -- vstup se zm�n� za 20 nanosekund
                  wait for 20 ns;		
               end loop;
            end loop;
         end loop;
      end loop;
      
      -- nekone�n� �ekac� smy�ka
      wait;									
		
   end process;

END;