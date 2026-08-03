<!--  Popup-Kalender -->
<style type="text/css">
  @import url(css/popcalendar.css);
</style>
<style>
    #dropzone {
      width: 300px;
      height: 200px;
      border: 2px dashed #888;
      border-radius: 8px;
      display: flex;
      align-items: center;
      justify-content: center;
      text-align: center;
      color: #666;
    }
    #dropzone.highlight {
      border-color: #2a8cff;
      color: #2a8cff;
    }
    #preview img {
      max-width: 150px;
      max-height: 150px;
      margin: 5px;
    }
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>
<script>
  const dropzone  = document.getElementById('dropzone');
  const fileInput = document.getElementById('fileInput');
  const preview   = document.getElementById('preview');

  // Klick als Fallback
  dropzone.addEventListener('click', () => fileInput.click());
  fileInput.addEventListener('change', () => handleFiles(fileInput.files));

  // Standardverhalten für Drag&Drop verhindern
  ['dragenter', 'dragover', 'dragleave', 'drop'].forEach(eventName => {
    dropzone.addEventListener(eventName, e => {
      e.preventDefault();
      e.stopPropagation();
    }, false);
  });

  // Optik beim Drüberziehen
  ['dragenter', 'dragover'].forEach(eventName => {
    dropzone.addEventListener(eventName, () => {
      dropzone.classList.add('highlight');
    }, false);
  });

  ['dragleave', 'drop'].forEach(eventName => {
    dropzone.addEventListener(eventName, () => {
      dropzone.classList.remove('highlight');
    }, false);
  });

  // Dateien beim Drop verarbeiten
  dropzone.addEventListener('drop', e => {
    const dt    = e.dataTransfer;
    const files = dt.files;
    handleFiles(files);
  });

  function handleFiles(files) {
    preview.innerHTML = ''; // alte Vorschau löschen
    [...files].forEach(file => {
      if (!file.type.startsWith('image/')) return; // nur Bilder

      const img = document.createElement('img');
      const reader = new FileReader();

      reader.onload = ev => {
        img.src = ev.target.result;
        preview.appendChild(img);
      };

      reader.readAsDataURL(file);
    });
  }
</script>

<form action="?id={$id}" name="Auftragform" method="post">
<input type="hidden" name="Auftrag" value="{$Auftrag->Event_id}">
<h2>{$Auftrag->Titel}</h2>
Auftrag erstellt von {$Auftrag->getAutor()} am {$Auftrag->Datum|date_format:"%d.%m.%Y %H:%M"}
{if ( $Auftrag->wurdeGeaendert() )}
, zuletzt geändert am {$Auftrag->changed|date_format:"%d.%m.%Y %H:%M"}
{/if}
<br />
<label>Verantwortlich</label> 
<select name="Betroffen[]" multiple="multiple" size="10">
{html_options options=$Auftrag->getListOfGroupsAndUsers() selected=$Auftrag->getBetroffene()}
</select> Um mehrere Einträge zu markieren bitte Strg gedrückt halten.<br />

<label>Titel</label><br /><input type="Text" name="Titel" value="{$Auftrag->Titel}" size="50" maxlength="50" /><br />
<label>Beschreibung</label><br /><textarea name="Beschreibung" cols="60" rows="5">{$Auftrag->Beschreibung}</textarea><br />

<label>Priorität</label> 
<select name="Prioritaet">
{html_options options=$Auftrag->getPrioritaeten() selected=$Auftrag->Prioritaet} 
</select>
&nbsp;&nbsp;&nbsp;&nbsp;<label>Status</label> 
<select name="Status">
{html_options options=$Auftrag->getStatusse() selected=$Auftrag->Status} 
</select><br />
<label for="BearbeitenBis">Bearbeiten bis</label>
<input type="text" name="BearbeitenBis" 
     onClick="popUpCalendar(this,this,'dd.mm.yyyy')" 
     onBlur="autoCorrectDate('Auftragform','BearbeitenBis',false);"          
 /> &nbsp;&nbsp;
<label for="Ort">Ort</label> 
<input type="hidden" name="Ort" value="{$Auftrag->Ort}" id="Ortsauswahl" />
{if ( isset($smarty.request.MyWork) )}
	<input type="hidden" name="MyWork" value="1" />
{/if}
<span name="Ortname" id="Ortsauswahltext">{$Auftrag->getOrt(true)}</span>
&nbsp;&nbsp;[ <a href="javaScript:OrtsAuswahl('Ortsauswahl')">Ort auswählen</a> ]<br />
<label>Visualisierung</label><br />
<div id="dropzone">Bild hierher ziehen oder klicken</div>
<input type="file" id="fileInput" accept="image/*" multiple style="display:none">
<div id="preview"></div>
{if ( ! isset($smarty.request.newauf))}
	<label>Bemerkungen</label><br />
	<textarea name="Bemerkung" cols="60" rows="5">{$Auftrag->Bemerkung}</textarea><br />
{/if}
<input type="Submit" name="saveauf" value="Speichern" />
{if ( !$Auftrag->isNeu() && $Auftrag->Autor == SESSION_DBID )}
	&nbsp;&nbsp;&nbsp;&nbsp;[ <a title="Löschen" href="?id={$id}&Delete={$Auftrag->Event_id}">Löschen</a> ]
{/if}
&nbsp;&nbsp;&nbsp;&nbsp;[ <a title="Auftragsliste anzeigen" href="?id={$id}">Auftragsliste anzeigen</a> ]
</form>
