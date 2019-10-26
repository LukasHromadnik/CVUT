<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3e" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="D" />
        <signal name="XLXN_2" />
        <signal name="E" />
        <signal name="XLXN_4" />
        <signal name="XLXN_5" />
        <signal name="XLXN_6" />
        <signal name="Q1" />
        <signal name="XLXN_7" />
        <signal name="Q2" />
        <port polarity="Input" name="D" />
        <port polarity="Input" name="E" />
        <port polarity="Output" name="Q1" />
        <port polarity="Output" name="Q2" />
        <blockdef name="nand2">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-64" y2="-64" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="216" y1="-96" y2="-96" x1="256" />
            <circle r="12" cx="204" cy="-96" />
            <line x2="64" y1="-48" y2="-144" x1="64" />
            <line x2="144" y1="-144" y2="-144" x1="64" />
            <line x2="64" y1="-48" y2="-48" x1="144" />
            <arc ex="144" ey="-144" sx="144" sy="-48" r="48" cx="144" cy="-96" />
        </blockdef>
        <block symbolname="nand2" name="XLXI_1">
            <blockpin signalname="E" name="I0" />
            <blockpin signalname="D" name="I1" />
            <blockpin signalname="XLXN_5" name="O" />
        </block>
        <block symbolname="nand2" name="XLXI_2">
            <blockpin signalname="E" name="I0" />
            <blockpin signalname="XLXN_5" name="I1" />
            <blockpin signalname="XLXN_7" name="O" />
        </block>
        <block symbolname="nand2" name="XLXI_3">
            <blockpin signalname="Q2" name="I0" />
            <blockpin signalname="XLXN_5" name="I1" />
            <blockpin signalname="Q1" name="O" />
        </block>
        <block symbolname="nand2" name="XLXI_5">
            <blockpin signalname="XLXN_7" name="I0" />
            <blockpin signalname="Q1" name="I1" />
            <blockpin signalname="Q2" name="O" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="1232" y="1248" name="XLXI_1" orien="R0" />
        <instance x="1232" y="1520" name="XLXI_2" orien="R0" />
        <instance x="1712" y="1248" name="XLXI_3" orien="R0" />
        <instance x="1696" y="1536" name="XLXI_5" orien="R0" />
        <branch name="D">
            <wire x2="1232" y1="1120" y2="1120" x1="976" />
        </branch>
        <branch name="E">
            <wire x2="1104" y1="1456" y2="1456" x1="992" />
            <wire x2="1232" y1="1456" y2="1456" x1="1104" />
            <wire x2="1232" y1="1184" y2="1184" x1="1104" />
            <wire x2="1104" y1="1184" y2="1456" x1="1104" />
        </branch>
        <branch name="XLXN_5">
            <wire x2="1232" y1="1392" y2="1392" x1="1168" />
            <wire x2="1168" y1="1392" y2="1552" x1="1168" />
            <wire x2="1520" y1="1552" y2="1552" x1="1168" />
            <wire x2="1520" y1="1152" y2="1152" x1="1488" />
            <wire x2="1600" y1="1152" y2="1152" x1="1520" />
            <wire x2="1520" y1="1152" y2="1552" x1="1520" />
            <wire x2="1600" y1="1120" y2="1152" x1="1600" />
            <wire x2="1712" y1="1120" y2="1120" x1="1600" />
        </branch>
        <branch name="Q1">
            <wire x2="1696" y1="1408" y2="1408" x1="1632" />
            <wire x2="1632" y1="1408" y2="1568" x1="1632" />
            <wire x2="2048" y1="1568" y2="1568" x1="1632" />
            <wire x2="2048" y1="1152" y2="1152" x1="1968" />
            <wire x2="2048" y1="1152" y2="1568" x1="2048" />
            <wire x2="2240" y1="1152" y2="1152" x1="2048" />
        </branch>
        <branch name="XLXN_7">
            <wire x2="1584" y1="1424" y2="1424" x1="1488" />
            <wire x2="1584" y1="1424" y2="1472" x1="1584" />
            <wire x2="1696" y1="1472" y2="1472" x1="1584" />
        </branch>
        <branch name="Q2">
            <wire x2="1648" y1="1040" y2="1184" x1="1648" />
            <wire x2="1712" y1="1184" y2="1184" x1="1648" />
            <wire x2="2032" y1="1040" y2="1040" x1="1648" />
            <wire x2="2032" y1="1040" y2="1440" x1="2032" />
            <wire x2="2224" y1="1440" y2="1440" x1="2032" />
            <wire x2="2032" y1="1440" y2="1440" x1="1952" />
        </branch>
        <iomarker fontsize="28" x="976" y="1120" name="D" orien="R180" />
        <iomarker fontsize="28" x="992" y="1456" name="E" orien="R180" />
        <iomarker fontsize="28" x="2240" y="1152" name="Q1" orien="R0" />
        <iomarker fontsize="28" x="2224" y="1440" name="Q2" orien="R0" />
    </sheet>
</drawing>