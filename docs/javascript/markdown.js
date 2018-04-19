function convert_markdown (){
    
    var converter = new showdown.Converter();
    var markdowns = document.getElementsByClassName("markdown");

    for (var i = 0, len = markdowns.length; i < len; i++)
    {
        myUrl = markdowns[i].getAttribute("md_file")

        var markdown_text = "";

        $.ajax( { url: myUrl,
                  type: 'get',
                  dataType: 'text',
                  async: false,
                  success: function(data) { markdown_text = data; }
        });

        markdowns[i].innerHTML = converter.makeHtml(markdown_text);
    }
}
