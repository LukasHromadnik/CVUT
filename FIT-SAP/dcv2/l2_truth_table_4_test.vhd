library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
  
-- prázdná entita zapouzdøující testovací sadu
ENTITY l2_truth_table_4_test IS
END l2_truth_table_4_test;
 
-- popis chování testovací sady
-- hlavièka ve formatu: ARCHITECTURE &lt;libovolny_nazev&gt; OF &lt;nazev_entity&gt; IS
ARCHITECTURE behavior OF l2_truth_table_4_test IS 
 
   -- definice vstupù a výstupù testované komponenty
   -- vstupy:  a, b, c, d
   -- výstupy: f
   -- POZOR!
   -- NÁZEV KOMPONENTY (COMPONENT &lt;nazev&gt;) MUSÍ BÝT STEJNÝ JAKO NÁZEV TESTOVANÉ ENTITY
   -- V TOMTO PØÍPADÌ: COMPONENT truth_table
   -- VSTUPY A VÝSTUPY KOMPONENTY (COMPONENT truth_table) MUSÍ BÝT STEJNÉ JAKO POPISY VSTUPÙ A VÝSTUPÙ TESTOVANÉ ENTITY
   COMPONENT truth_table
   PORT(
     d : IN  std_logic;
     c : IN  std_logic;
     b : IN  std_logic;
     a : IN  std_logic;
     f : OUT  std_logic
   );
   END COMPONENT;
    
   -- vstupní signály ovládané testovací sadou
   signal d : std_logic := '0';
   signal c : std_logic := '0';
   signal b : std_logic := '0';
   signal a : std_logic := '0';

   -- výstupní signály ovládané testovací sadou
   signal f : std_logic;
 
BEGIN
 
   -- mapování vstupù a výstupù testované komponenty na signály ovládané testovací sadou
   uut: truth_table PORT MAP (
      d => d,
      c => c,
      b => b,
      a => a,
      f => f
   );
 
   -- simulaèní proces realizující chování testovací sady
   stim_proc: process
      -- deklarace promìnných pro práci s for-cykly
      variable i, j, k, l  :  integer;
   begin		
      -- poèáteèní nastavení vstupù
      d   <=   '0';
      c   <=   '0';
      b   <=   '0';
      a   <=   '0';
		
      -- testování všech možných kombinací
      -- 4 vstupy =&gt; 4 for-cykly =&gt; 2^4 možných vstupních kombinací
      for i in 0 to 1 loop	
         d   <=   not(d);
         for j in 0 to 1 loop
            c   <=   not(c);
            for k in 0 to 1 loop
               b   <=   not(b);
               for l in 0 to 1 loop
                  a   <=   not(a);
                  -- vstup se zmìní za 20 nanosekund
                  wait for 20 ns;		
               end loop;
            end loop;
         end loop;
      end loop;
      
      -- nekoneèná èekací smyèka
      wait;									
		
   end process;

END;