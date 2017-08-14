<html>
        <head>
          <title>Tutorial</title>
          <script type="text/javascript">
                  function reloadimg(){
                    uniq=new Date();
                    uniq="?"+uniq.getTime();
                    var target="imgToLoad";
                    newImage1=document.images[target].src;
                    index1=newImage1.indexOf("?", 0);
                    if(index1>0){
                        newImage1=newImage1.substr(0, index1);
                    }
                    document.images[target].src=newImage1+uniq;
                    setTimeout('reloadimg()', 33);
                  }
          </script>
        </head>
        <body onload="reloadimg()">
          <img src="TCPimage.jpg" name="imgToLoad"/><br/>

        </body>
</html>

