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
        <signal name="XLXN_4" />
        <signal name="XLXN_5" />
        <signal name="XLXN_6" />
        <signal name="XLXN_7" />
        <signal name="XLXN_11" />
        <signal name="XLXN_12" />
        <signal name="XLXN_13" />
        <signal name="A" />
        <signal name="B" />
        <signal name="C" />
        <signal name="D" />
        <signal name="XLXN_19" />
        <signal name="XLXN_20" />
        <signal name="XLXN_21" />
        <signal name="XLXN_22" />
        <signal name="XLXN_23" />
        <signal name="XLXN_25" />
        <signal name="F" />
        <port polarity="Input" name="A" />
        <port polarity="Input" name="B" />
        <port polarity="Input" name="C" />
        <port polarity="Input" name="D" />
        <port polarity="Output" name="F" />
        <blockdef name="and3">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-64" y2="-64" x1="0" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <line x2="192" y1="-128" y2="-128" x1="256" />
            <line x2="144" y1="-176" y2="-176" x1="64" />
            <line x2="64" y1="-80" y2="-80" x1="144" />
            <arc ex="144" ey="-176" sx="144" sy="-80" r="48" cx="144" cy="-128" />
            <line x2="64" y1="-64" y2="-192" x1="64" />
        </blockdef>
        <blockdef name="or6">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="48" y1="-64" y2="-64" x1="0" />
            <line x2="48" y1="-128" y2="-128" x1="0" />
            <line x2="48" y1="-320" y2="-320" x1="0" />
            <line x2="48" y1="-384" y2="-384" x1="0" />
            <line x2="192" y1="-224" y2="-224" x1="256" />
            <line x2="64" y1="-256" y2="-256" x1="0" />
            <line x2="64" y1="-192" y2="-192" x1="0" />
            <arc ex="112" ey="-272" sx="192" sy="-224" r="88" cx="116" cy="-184" />
            <line x2="48" y1="-176" y2="-176" x1="112" />
            <arc ex="192" ey="-224" sx="112" sy="-176" r="88" cx="116" cy="-264" />
            <arc ex="48" ey="-272" sx="48" sy="-176" r="56" cx="16" cy="-224" />
            <line x2="48" y1="-272" y2="-272" x1="112" />
            <line x2="48" y1="-64" y2="-176" x1="48" />
            <line x2="48" y1="-272" y2="-384" x1="48" />
        </blockdef>
        <blockdef name="inv">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="160" y1="-32" y2="-32" x1="224" />
            <line x2="128" y1="-64" y2="-32" x1="64" />
            <line x2="64" y1="-32" y2="0" x1="128" />
            <line x2="64" y1="0" y2="-64" x1="64" />
            <circle r="16" cx="144" cy="-32" />
        </blockdef>
        <block symbolname="and3" name="XLXI_1">
            <blockpin signalname="XLXN_3" name="I0" />
            <blockpin signalname="XLXN_2" name="I1" />
            <blockpin signalname="XLXN_1" name="I2" />
            <blockpin signalname="XLXN_19" name="O" />
        </block>
        <block symbolname="and3" name="XLXI_2">
            <blockpin signalname="A" name="I0" />
            <blockpin signalname="XLXN_5" name="I1" />
            <blockpin signalname="XLXN_4" name="I2" />
            <blockpin signalname="XLXN_20" name="O" />
        </block>
        <block symbolname="and3" name="XLXI_3">
            <blockpin signalname="A" name="I0" />
            <blockpin signalname="B" name="I1" />
            <blockpin signalname="C" name="I2" />
            <blockpin signalname="XLXN_21" name="O" />
        </block>
        <block symbolname="and3" name="XLXI_4">
            <blockpin signalname="XLXN_6" name="I0" />
            <blockpin signalname="B" name="I1" />
            <blockpin signalname="C" name="I2" />
            <blockpin signalname="XLXN_22" name="O" />
        </block>
        <block symbolname="and3" name="XLXI_5">
            <blockpin signalname="XLXN_11" name="I0" />
            <blockpin signalname="XLXN_7" name="I1" />
            <blockpin signalname="C" name="I2" />
            <blockpin signalname="XLXN_23" name="O" />
        </block>
        <block symbolname="and3" name="XLXI_6">
            <blockpin signalname="XLXN_13" name="I0" />
            <blockpin signalname="XLXN_12" name="I1" />
            <blockpin signalname="A" name="I2" />
            <blockpin signalname="XLXN_25" name="O" />
        </block>
        <block symbolname="or6" name="XLXI_7">
            <blockpin signalname="XLXN_25" name="I0" />
            <blockpin signalname="XLXN_23" name="I1" />
            <blockpin signalname="XLXN_22" name="I2" />
            <blockpin signalname="XLXN_21" name="I3" />
            <blockpin signalname="XLXN_20" name="I4" />
            <blockpin signalname="XLXN_19" name="I5" />
            <blockpin signalname="F" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_8">
            <blockpin signalname="C" name="I" />
            <blockpin signalname="XLXN_1" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_9">
            <blockpin signalname="B" name="I" />
            <blockpin signalname="XLXN_2" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_11">
            <blockpin signalname="D" name="I" />
            <blockpin signalname="XLXN_3" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_12">
            <blockpin signalname="C" name="I" />
            <blockpin signalname="XLXN_4" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_13">
            <blockpin signalname="D" name="I" />
            <blockpin signalname="XLXN_5" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_14">
            <blockpin signalname="D" name="I" />
            <blockpin signalname="XLXN_6" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_15">
            <blockpin signalname="A" name="I" />
            <blockpin signalname="XLXN_7" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_19">
            <blockpin signalname="B" name="I" />
            <blockpin signalname="XLXN_11" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_20">
            <blockpin signalname="C" name="I" />
            <blockpin signalname="XLXN_12" name="O" />
        </block>
        <block symbolname="inv" name="XLXI_21">
            <blockpin signalname="D" name="I" />
            <blockpin signalname="XLXN_13" name="O" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="912" y="560" name="XLXI_1" orien="R0" />
        <instance x="944" y="944" name="XLXI_2" orien="R0" />
        <instance x="944" y="1312" name="XLXI_3" orien="R0" />
        <instance x="896" y="1664" name="XLXI_4" orien="R0" />
        <instance x="912" y="1984" name="XLXI_5" orien="R0" />
        <instance x="896" y="2288" name="XLXI_6" orien="R0" />
        <instance x="1728" y="1376" name="XLXI_7" orien="R0" />
        <branch name="XLXN_1">
            <wire x2="912" y1="368" y2="368" x1="880" />
        </branch>
        <instance x="656" y="400" name="XLXI_8" orien="R0" />
        <branch name="XLXN_2">
            <wire x2="912" y1="432" y2="432" x1="880" />
        </branch>
        <instance x="656" y="464" name="XLXI_9" orien="R0" />
        <branch name="XLXN_3">
            <wire x2="912" y1="496" y2="496" x1="880" />
        </branch>
        <instance x="656" y="528" name="XLXI_11" orien="R0" />
        <branch name="XLXN_4">
            <wire x2="944" y1="752" y2="752" x1="912" />
        </branch>
        <instance x="688" y="784" name="XLXI_12" orien="R0" />
        <branch name="XLXN_5">
            <wire x2="944" y1="816" y2="816" x1="912" />
        </branch>
        <instance x="688" y="848" name="XLXI_13" orien="R0" />
        <branch name="XLXN_6">
            <wire x2="896" y1="1600" y2="1600" x1="864" />
        </branch>
        <instance x="640" y="1632" name="XLXI_14" orien="R0" />
        <branch name="XLXN_7">
            <wire x2="912" y1="1856" y2="1856" x1="880" />
        </branch>
        <instance x="656" y="1888" name="XLXI_15" orien="R0" />
        <branch name="XLXN_11">
            <wire x2="912" y1="1920" y2="1920" x1="880" />
        </branch>
        <instance x="656" y="1952" name="XLXI_19" orien="R0" />
        <branch name="XLXN_12">
            <wire x2="896" y1="2160" y2="2160" x1="864" />
        </branch>
        <instance x="640" y="2192" name="XLXI_20" orien="R0" />
        <branch name="XLXN_13">
            <wire x2="896" y1="2224" y2="2224" x1="864" />
        </branch>
        <instance x="640" y="2256" name="XLXI_21" orien="R0" />
        <branch name="XLXN_19">
            <wire x2="1728" y1="432" y2="432" x1="1168" />
            <wire x2="1728" y1="432" y2="992" x1="1728" />
        </branch>
        <branch name="XLXN_20">
            <wire x2="1456" y1="816" y2="816" x1="1200" />
            <wire x2="1456" y1="816" y2="1056" x1="1456" />
            <wire x2="1728" y1="1056" y2="1056" x1="1456" />
        </branch>
        <branch name="XLXN_21">
            <wire x2="1456" y1="1184" y2="1184" x1="1200" />
            <wire x2="1456" y1="1120" y2="1184" x1="1456" />
            <wire x2="1728" y1="1120" y2="1120" x1="1456" />
        </branch>
        <branch name="XLXN_22">
            <wire x2="1472" y1="1536" y2="1536" x1="1152" />
            <wire x2="1472" y1="1184" y2="1536" x1="1472" />
            <wire x2="1728" y1="1184" y2="1184" x1="1472" />
        </branch>
        <branch name="XLXN_23">
            <wire x2="1488" y1="1856" y2="1856" x1="1168" />
            <wire x2="1488" y1="1248" y2="1856" x1="1488" />
            <wire x2="1728" y1="1248" y2="1248" x1="1488" />
        </branch>
        <branch name="XLXN_25">
            <wire x2="1728" y1="2160" y2="2160" x1="1152" />
            <wire x2="1728" y1="1312" y2="2160" x1="1728" />
        </branch>
        <iomarker fontsize="28" x="144" y="192" name="A" orien="R270" />
        <iomarker fontsize="28" x="272" y="192" name="B" orien="R270" />
        <iomarker fontsize="28" x="400" y="192" name="C" orien="R270" />
        <iomarker fontsize="28" x="496" y="192" name="D" orien="R270" />
        <branch name="F">
            <wire x2="2016" y1="1152" y2="1152" x1="1984" />
        </branch>
        <iomarker fontsize="28" x="2016" y="1152" name="F" orien="R0" />
        <branch name="D">
            <wire x2="496" y1="192" y2="496" x1="496" />
            <wire x2="656" y1="496" y2="496" x1="496" />
            <wire x2="496" y1="496" y2="816" x1="496" />
            <wire x2="688" y1="816" y2="816" x1="496" />
            <wire x2="496" y1="816" y2="1600" x1="496" />
            <wire x2="640" y1="1600" y2="1600" x1="496" />
            <wire x2="496" y1="1600" y2="2224" x1="496" />
            <wire x2="640" y1="2224" y2="2224" x1="496" />
        </branch>
        <branch name="C">
            <wire x2="400" y1="192" y2="368" x1="400" />
            <wire x2="656" y1="368" y2="368" x1="400" />
            <wire x2="400" y1="368" y2="752" x1="400" />
            <wire x2="688" y1="752" y2="752" x1="400" />
            <wire x2="400" y1="752" y2="1120" x1="400" />
            <wire x2="944" y1="1120" y2="1120" x1="400" />
            <wire x2="400" y1="1120" y2="1472" x1="400" />
            <wire x2="896" y1="1472" y2="1472" x1="400" />
            <wire x2="400" y1="1472" y2="1792" x1="400" />
            <wire x2="912" y1="1792" y2="1792" x1="400" />
            <wire x2="400" y1="1792" y2="2160" x1="400" />
            <wire x2="640" y1="2160" y2="2160" x1="400" />
        </branch>
        <branch name="B">
            <wire x2="272" y1="192" y2="432" x1="272" />
            <wire x2="656" y1="432" y2="432" x1="272" />
            <wire x2="272" y1="432" y2="1184" x1="272" />
            <wire x2="944" y1="1184" y2="1184" x1="272" />
            <wire x2="272" y1="1184" y2="1536" x1="272" />
            <wire x2="896" y1="1536" y2="1536" x1="272" />
            <wire x2="272" y1="1536" y2="1920" x1="272" />
            <wire x2="656" y1="1920" y2="1920" x1="272" />
        </branch>
        <branch name="A">
            <wire x2="144" y1="192" y2="880" x1="144" />
            <wire x2="944" y1="880" y2="880" x1="144" />
            <wire x2="144" y1="880" y2="1248" x1="144" />
            <wire x2="944" y1="1248" y2="1248" x1="144" />
            <wire x2="144" y1="1248" y2="1856" x1="144" />
            <wire x2="656" y1="1856" y2="1856" x1="144" />
            <wire x2="144" y1="1856" y2="2096" x1="144" />
            <wire x2="896" y1="2096" y2="2096" x1="144" />
        </branch>
    </sheet>
</drawing>