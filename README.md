# Arduino-dutch-traffic-lights
This is an Arduino project for the traffic lights system in the Netherlands. The system is composed of 2 boards where we have one Master and one Slave. The Master controls the slave with the following signals, which are send on time intervals:
<ol>
  <li> RED </li>
  <ul>
    <li> This signal is send to the slave in order for it to change to RED</li>
  </ul>
  <li> YELLOW </li>
  <ul>
    <li> This signal is send to the slave in order for it to change to YELLOW but this yellow is the fast one which blinks for just a moment and switches to GREEN</li>
  </ul>
  <li> GREEN </li>
  <ul>
    <li> This signal is send to the slave in order for it to change to GREEN</li>
  </ul>
  <li> LONG </li>
  <ul>
    <li> This signal is send to the slave in order for it to change to YELLOW but this is the yellow with longer duration which is placed right before switch to RED</li>
  </ul>
 </ol>
 The Slave board receives all the noted signals and if the signal is read, the Slave sends an ACK (acknowledge) signal to the Master that it has received the signal and executed the specified instruction.
