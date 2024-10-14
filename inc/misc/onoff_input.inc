<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

?>

<?=$title?><br />
<input type="radio" value="1" name="docdata[<?=$value?>]"<?php if($document[$value] == '1') echo ' CHECKED'; ?> /> <?=LANG_YES?>&nbsp;&nbsp;
<input type="radio" value="0" name="docdata[<?=$value?>]"<?php if($document[$value] == '0') echo ' CHECKED'; ?> /> <?=LANG_NO?>