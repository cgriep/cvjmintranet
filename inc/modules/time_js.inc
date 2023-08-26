<?php
/*
<!--
// paul changed new LiveClock('arial,1,#dbb600,,<b>,</b>,100,0,2,0,1,0');
// to           new LiveClock('arial,1,<?php echo $design['textcolor']; ?>,,<b>,</b>,100,0,2,0,1,0');
// 15/11/2002
-->

<?php echo $design['textcolor']; ?>
<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" width="100%" background="http://www.awfers.org/img/world.gif">

*/
?>
<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" width="100%">
 <tr>
      <td width="40%"><font size="1" color="<?php echo $design['textcolor']; ?>">GMT:</font></td>
      <td width="60%" align="right"><script language="javascript"><!--
//new LiveClock('font,size,color,background,start,end,width,12hour,update,abbrev,showdate,gmt');
new LiveClock('arial,1,<?php echo $design['textcolor']; ?>,,<b>,</b>,100,0,2,0,1,0');
//--></script></td>
   </tr>
    <tr>
      <td width="40%"><font size="1" color="<?php echo $design['textcolor']; ?>">Los Angeles:</font></td>
      <td width="60%" align="right"><script language="javascript"><!--
//new LiveClock('font,size,color,background,start,end,width,12hour,update,abbrev,showdate,gmt');
new LiveClock('arial,1,<?php echo $design['textcolor']; ?>,,<b>,</b>,100,0,2,0,1,-8');
//--></script></td>
   </tr>  
     <tr>
      <td width="40%"><font size="1" color="<?php echo $design['textcolor']; ?>">New York:</font></td>
      <td width="60%" align="right"><script language="javascript"><!--
//new LiveClock('font,size,color,background,start,end,width,12hour,update,abbrev,showdate,gmt');
new LiveClock('arial,1,<?php echo $design['textcolor']; ?>,,<b>,</b>,100,0,2,0,1,-4');
//--></script></td>
   </tr>
   <tr>
      <td width="40%"><font size="1" color="<?php echo $design['textcolor']; ?>">Berlin: </font> </td>
      <td width="60%" align="right"><script language="javascript"><!--
//new LiveClock('font,size,color,background,start,end,width,12hour,update,abbrev,showdate,gmt');
new LiveClock('arial,1,<?php echo $design['textcolor']; ?>,,<b>,</b>,100,0,2,0,1,+2');
//--></script></td>
   </tr>
  <tr>
      <td width="40%"><font size="1" color="<?php echo $design['textcolor']; ?>">Moskau:</font></td>
      <td width="60%" align="right"><script language="javascript"><!--
//new LiveClock('font,size,color,background,start,end,width,12hour,update,abbrev,showdate,gmt');
new LiveClock('arial,1,<?php echo $design['textcolor']; ?>,,<b>,</b>,100,0,2,0,1,+4');
//--></script></td>
   </tr>   
     <tr>
      <td width="40%"><font size="1" color="<?php echo $design['textcolor']; ?>">Tokyo:</font></td>
      <td width="60%" align="right"><script language="javascript"><!--
//new LiveClock('font,size,color,background,start,end,width,12hour,update,abbrev,showdate,gmt');
new LiveClock('arial,1,<?php echo $design['textcolor']; ?>,,<b>,</b>,100,0,2,0,1,+9');
//--></script></td>
   </tr>   
   <tr>
      <td width="40%"><font size="1" color="<?php echo $design['textcolor']; ?>">Sydney:</font></td>
      <td width="60%" align="right"><script language="javascript"><!--
//new LiveClock('font,size,color,background,start,end,width,12hour,update,abbrev,showdate,gmt');
new LiveClock('arial,1,<?php echo $design['textcolor']; ?>,,<b>,</b>,100,0,2,0,1,+10');
//--></script></td>
   </tr></table>

