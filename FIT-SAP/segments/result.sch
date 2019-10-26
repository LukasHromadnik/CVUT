<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3e" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="A" />
        <signal name="B" />
        <signal name="C" />
        <signal name="D" />
        <signal name="P" />
        <signal name="Q" />
        <signal name="R" />
        <signal name="S" />
        <signal name="T" />
        <signal name="U" />
        <signal name="V" />
        <port polarity="Input" name="A" />
        <port polarity="Input" name="B" />
        <port polarity="Input" name="C" />
        <port polarity="Input" name="D" />
        <port polarity="Output" name="P" />
        <port polarity="Output" name="Q" />
        <port polarity="Output" name="R" />
        <port polarity="Output" name="S" />
        <port polarity="Output" name="T" />
        <port polarity="Output" name="U" />
        <port polarity="Output" name="V" />
        <blockdef name="segment">
            <timestamp>2014-3-2T12:43:1</timestamp>
            <rect width="256" x="64" y="-448" height="448" />
            <line x2="0" y1="-416" y2="-416" x1="64" />
            <line x2="0" y1="-352" y2="-352" x1="64" />
            <line x2="0" y1="-288" y2="-288" x1="64" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="384" y1="-416" y2="-416" x1="320" />
            <line x2="384" y1="-352" y2="-352" x1="320" />
            <line x2="384" y1="-288" y2="-288" x1="320" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
            <line x2="384" y1="-160" y2="-160" x1="320" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
        </blockdef>
        <block symbolname="segment" name="XLXI_1">
            <blockpin signalname="A" name="A" />
            <blockpin signalname="B" name="B" />
            <blockpin signalname="C" name="C" />
            <blockpin signalname="D" name="D" />
            <blockpin signalname="P" name="P" />
            <blockpin signalname="Q" name="Q" />
            <blockpin signalname="R" name="R" />
            <blockpin signalname="S" name="S" />
            <blockpin signalname="T" name="T" />
            <blockpin signalname="U" name="U" />
            <blockpin signalname="V" name="V" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="720" y="976" name="XLXI_1" orien="R0">
        </instance>
        <branch name="A">
            <wire x2="720" y1="560" y2="560" x1="688" />
        </branch>
        <iomarker fontsize="28" x="688" y="560" name="A" orien="R180" />
        <branch name="B">
            <wire x2="720" y1="624" y2="624" x1="688" />
        </branch>
        <iomarker fontsize="28" x="688" y="624" name="B" orien="R180" />
        <branch name="C">
            <wire x2="720" y1="688" y2="688" x1="688" />
        </branch>
        <iomarker fontsize="28" x="688" y="688" name="C" orien="R180" />
        <branch name="D">
            <wire x2="720" y1="752" y2="752" x1="688" />
        </branch>
        <iomarker fontsize="28" x="688" y="752" name="D" orien="R180" />
        <branch name="P">
            <wire x2="1136" y1="560" y2="560" x1="1104" />
        </branch>
        <iomarker fontsize="28" x="1136" y="560" name="P" orien="R0" />
        <branch name="Q">
            <wire x2="1136" y1="624" y2="624" x1="1104" />
        </branch>
        <iomarker fontsize="28" x="1136" y="624" name="Q" orien="R0" />
        <branch name="R">
            <wire x2="1136" y1="688" y2="688" x1="1104" />
        </branch>
        <iomarker fontsize="28" x="1136" y="688" name="R" orien="R0" />
        <branch name="S">
            <wire x2="1136" y1="752" y2="752" x1="1104" />
        </branch>
        <iomarker fontsize="28" x="1136" y="752" name="S" orien="R0" />
        <branch name="T">
            <wire x2="1136" y1="816" y2="816" x1="1104" />
        </branch>
        <iomarker fontsize="28" x="1136" y="816" name="T" orien="R0" />
        <branch name="U">
            <wire x2="1136" y1="880" y2="880" x1="1104" />
        </branch>
        <iomarker fontsize="28" x="1136" y="880" name="U" orien="R0" />
        <branch name="V">
            <wire x2="1136" y1="944" y2="944" x1="1104" />
        </branch>
        <iomarker fontsize="28" x="1136" y="944" name="V" orien="R0" />
    </sheet>
</drawing>