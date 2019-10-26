<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3e" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="XLXN_1" />
        <signal name="XLXN_2" />
        <signal name="XLXN_3" />
        <signal name="Q0_IN" />
        <signal name="XLXN_6" />
        <signal name="XLXN_7" />
        <signal name="CLR" />
        <signal name="XLXN_9" />
        <signal name="Q1_IN" />
        <signal name="Q2_IN" />
        <signal name="CLK" />
        <signal name="BTN_CLK" />
        <signal name="X" />
        <signal name="XLXN_17" />
        <signal name="XLXN_18" />
        <signal name="XLXN_19" />
        <signal name="XLXN_20" />
        <signal name="XLXN_21" />
        <port polarity="Output" name="Q0_IN" />
        <port polarity="Input" name="CLR" />
        <port polarity="Output" name="Q1_IN" />
        <port polarity="Output" name="Q2_IN" />
        <port polarity="Input" name="CLK" />
        <port polarity="Input" name="BTN_CLK" />
        <port polarity="Input" name="X" />
        <blockdef name="q0">
            <timestamp>2014-3-17T8:51:24</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="q1">
            <timestamp>2014-3-17T8:51:28</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="q2">
            <timestamp>2014-3-17T8:51:54</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="fdc">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="64" y1="-256" y2="-256" x1="0" />
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <rect width="256" x="64" y="-320" height="256" />
            <line x2="80" y1="-112" y2="-128" x1="64" />
            <line x2="64" y1="-128" y2="-144" x1="80" />
            <line x2="192" y1="-64" y2="-32" x1="192" />
            <line x2="64" y1="-32" y2="-32" x1="192" />
        </blockdef>
        <blockdef name="debounce">
            <timestamp>2014-3-17T8:50:42</timestamp>
            <rect width="256" x="64" y="-128" height="128" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
        </blockdef>
        <block symbolname="q0" name="XLXI_1">
            <blockpin signalname="X" name="d" />
            <blockpin signalname="Q2_IN" name="c" />
            <blockpin signalname="Q1_IN" name="b" />
            <blockpin signalname="Q0_IN" name="a" />
            <blockpin signalname="XLXN_1" name="f" />
        </block>
        <block symbolname="q1" name="XLXI_2">
            <blockpin signalname="X" name="d" />
            <blockpin signalname="Q2_IN" name="c" />
            <blockpin signalname="Q1_IN" name="b" />
            <blockpin signalname="Q0_IN" name="a" />
            <blockpin signalname="XLXN_2" name="f" />
        </block>
        <block symbolname="q2" name="XLXI_3">
            <blockpin signalname="X" name="d" />
            <blockpin signalname="Q2_IN" name="c" />
            <blockpin signalname="Q1_IN" name="b" />
            <blockpin signalname="Q0_IN" name="a" />
            <blockpin signalname="XLXN_3" name="f" />
        </block>
        <block symbolname="fdc" name="XLXI_4">
            <blockpin signalname="XLXN_7" name="C" />
            <blockpin signalname="CLR" name="CLR" />
            <blockpin signalname="XLXN_3" name="D" />
            <blockpin signalname="Q2_IN" name="Q" />
        </block>
        <block symbolname="fdc" name="XLXI_5">
            <blockpin signalname="XLXN_7" name="C" />
            <blockpin signalname="CLR" name="CLR" />
            <blockpin signalname="XLXN_2" name="D" />
            <blockpin signalname="Q1_IN" name="Q" />
        </block>
        <block symbolname="fdc" name="XLXI_6">
            <blockpin signalname="XLXN_7" name="C" />
            <blockpin signalname="CLR" name="CLR" />
            <blockpin signalname="XLXN_1" name="D" />
            <blockpin signalname="Q0_IN" name="Q" />
        </block>
        <block symbolname="debounce" name="XLXI_7">
            <blockpin signalname="CLK" name="clk" />
            <blockpin signalname="BTN_CLK" name="tl_in" />
            <blockpin signalname="XLXN_7" name="tl_out" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="928" y="624" name="XLXI_1" orien="R0">
        </instance>
        <instance x="912" y="1088" name="XLXI_2" orien="R0">
        </instance>
        <instance x="912" y="1552" name="XLXI_3" orien="R0">
        </instance>
        <instance x="1472" y="1616" name="XLXI_4" orien="R0" />
        <instance x="1520" y="1136" name="XLXI_5" orien="R0" />
        <instance x="1536" y="672" name="XLXI_6" orien="R0" />
        <branch name="XLXN_1">
            <wire x2="1424" y1="400" y2="400" x1="1312" />
            <wire x2="1424" y1="400" y2="416" x1="1424" />
            <wire x2="1536" y1="416" y2="416" x1="1424" />
        </branch>
        <branch name="XLXN_2">
            <wire x2="1408" y1="864" y2="864" x1="1296" />
            <wire x2="1408" y1="864" y2="880" x1="1408" />
            <wire x2="1520" y1="880" y2="880" x1="1408" />
        </branch>
        <branch name="XLXN_3">
            <wire x2="1376" y1="1328" y2="1328" x1="1296" />
            <wire x2="1376" y1="1328" y2="1360" x1="1376" />
            <wire x2="1472" y1="1360" y2="1360" x1="1376" />
        </branch>
        <branch name="Q0_IN">
            <wire x2="624" y1="64" y2="592" x1="624" />
            <wire x2="624" y1="592" y2="704" x1="624" />
            <wire x2="1920" y1="704" y2="704" x1="624" />
            <wire x2="1936" y1="704" y2="704" x1="1920" />
            <wire x2="624" y1="704" y2="1056" x1="624" />
            <wire x2="912" y1="1056" y2="1056" x1="624" />
            <wire x2="624" y1="1056" y2="1520" x1="624" />
            <wire x2="912" y1="1520" y2="1520" x1="624" />
            <wire x2="928" y1="592" y2="592" x1="624" />
            <wire x2="1936" y1="416" y2="416" x1="1920" />
            <wire x2="1936" y1="416" y2="704" x1="1936" />
        </branch>
        <branch name="XLXN_7">
            <wire x2="1456" y1="1792" y2="1792" x1="512" />
            <wire x2="1456" y1="1008" y2="1488" x1="1456" />
            <wire x2="1472" y1="1488" y2="1488" x1="1456" />
            <wire x2="1456" y1="1488" y2="1792" x1="1456" />
            <wire x2="1504" y1="1008" y2="1008" x1="1456" />
            <wire x2="1520" y1="1008" y2="1008" x1="1504" />
            <wire x2="1504" y1="544" y2="1008" x1="1504" />
            <wire x2="1536" y1="544" y2="544" x1="1504" />
        </branch>
        <branch name="CLR">
            <wire x2="1392" y1="1232" y2="1584" x1="1392" />
            <wire x2="1472" y1="1584" y2="1584" x1="1392" />
            <wire x2="1392" y1="1584" y2="2176" x1="1392" />
            <wire x2="1488" y1="1232" y2="1232" x1="1392" />
            <wire x2="1488" y1="752" y2="1104" x1="1488" />
            <wire x2="1520" y1="1104" y2="1104" x1="1488" />
            <wire x2="1488" y1="1104" y2="1232" x1="1488" />
            <wire x2="1536" y1="752" y2="752" x1="1488" />
            <wire x2="1536" y1="640" y2="752" x1="1536" />
        </branch>
        <branch name="Q1_IN">
            <wire x2="480" y1="80" y2="528" x1="480" />
            <wire x2="928" y1="528" y2="528" x1="480" />
            <wire x2="480" y1="528" y2="992" x1="480" />
            <wire x2="480" y1="992" y2="1216" x1="480" />
            <wire x2="1904" y1="1216" y2="1216" x1="480" />
            <wire x2="1920" y1="1216" y2="1216" x1="1904" />
            <wire x2="480" y1="1216" y2="1456" x1="480" />
            <wire x2="912" y1="1456" y2="1456" x1="480" />
            <wire x2="912" y1="992" y2="992" x1="480" />
            <wire x2="1920" y1="880" y2="880" x1="1904" />
            <wire x2="1920" y1="880" y2="1216" x1="1920" />
        </branch>
        <branch name="Q2_IN">
            <wire x2="288" y1="96" y2="464" x1="288" />
            <wire x2="928" y1="464" y2="464" x1="288" />
            <wire x2="288" y1="464" y2="928" x1="288" />
            <wire x2="912" y1="928" y2="928" x1="288" />
            <wire x2="288" y1="928" y2="1392" x1="288" />
            <wire x2="288" y1="1392" y2="1712" x1="288" />
            <wire x2="1856" y1="1712" y2="1712" x1="288" />
            <wire x2="1872" y1="1712" y2="1712" x1="1856" />
            <wire x2="912" y1="1392" y2="1392" x1="288" />
            <wire x2="1872" y1="1360" y2="1360" x1="1856" />
            <wire x2="1872" y1="1360" y2="1712" x1="1872" />
        </branch>
        <instance x="128" y="1888" name="XLXI_7" orien="R0">
        </instance>
        <branch name="CLK">
            <wire x2="128" y1="1792" y2="1792" x1="96" />
        </branch>
        <iomarker fontsize="28" x="96" y="1792" name="CLK" orien="R180" />
        <branch name="BTN_CLK">
            <wire x2="128" y1="1856" y2="1856" x1="96" />
        </branch>
        <iomarker fontsize="28" x="96" y="1856" name="BTN_CLK" orien="R180" />
        <iomarker fontsize="28" x="1392" y="2176" name="CLR" orien="R90" />
        <branch name="X">
            <wire x2="112" y1="224" y2="400" x1="112" />
            <wire x2="928" y1="400" y2="400" x1="112" />
            <wire x2="112" y1="400" y2="864" x1="112" />
            <wire x2="912" y1="864" y2="864" x1="112" />
            <wire x2="112" y1="864" y2="1328" x1="112" />
            <wire x2="912" y1="1328" y2="1328" x1="112" />
        </branch>
        <iomarker fontsize="28" x="112" y="224" name="X" orien="R270" />
        <iomarker fontsize="28" x="288" y="96" name="Q2_IN" orien="R270" />
        <iomarker fontsize="28" x="480" y="80" name="Q1_IN" orien="R270" />
        <iomarker fontsize="28" x="624" y="64" name="Q0_IN" orien="R270" />
    </sheet>
</drawing>