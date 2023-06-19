var player1 = [];
  var cpu = [];
  function game(button)
  {
      if(button.childNodes.length==0)
      {
        var div = button;
        var h1 = document.createElement("h1");
        h1.innerText="X";
        h1.style.color="yellow";
        h1.style.textAlign="center";
        h1.style.fontSize = "6em";
        button.appendChild(h1);
        player1.push(parseInt(button.getAttribute("data-index")));
        var flag=false;
        if(player1.includes(0) && player1.includes(4) && player1.includes(8))
        {
             
          flag=true;;
        }
        else if(player1.includes(2) && player1.includes(4) && player1.includes(6))
        {
             
          flag=true;;
        }

        else if(player1.includes(0) && player1.includes(1) && player1.includes(2))
        {
             
          flag=true;;
        }
        else if(player1.includes(3) && player1.includes(4) && player1.includes(5))
        {
             
          flag=true;;
        }
        else if(player1.includes(6) && player1.includes(7) && player1.includes(8))
        {
             
          flag=true;;
        }
        else if(player1.includes(0) && player1.includes(3) && player1.includes(6))
        {
             
          flag=true;;
        }
        else if(player1.includes(1) && player1.includes(4) && player1.includes(7))
        {
             
          flag=true;;
        }

        else if(player1.includes(2) && player1.includes(5) && player1.includes(7))
        {
             
          flag=true;;
        }
        if(flag)
        {
          document.getElementById("result-header").innerText="Human Won!";
        }
        else if(player1.length+cpu.length==9)
        {
            document.getElementById("result-header").innerText="Game Tied!";

        }
        else
        {
          cpuMove();
        }
      }
    else
    {
      alert("This Place Is Already Marked");
    }
  }
  function cpuMove()
  {
      let random = parseInt(Math.floor(Math.random() * (8 - 0) + 0));
      while(cpu.includes(random) || player1.includes(random))
      {
        random = parseInt(Math.floor(Math.random() * (8 - 0) + 0));
        console.log("infinite loop");
        }
        cpu.push(random);
        var allDivs = document.querySelectorAll("div");
        for(var f=0;f<allDivs.length;f++)
        {
          if(allDivs[f].hasAttribute("data-index"))
          {
            if(parseInt(allDivs[f].getAttribute("data-index"))==random)
            {
              var hh1 = document.createElement("h1");
              hh1.innerText="O";
              hh1.style.color="vlue";
              hh1.style.textAlign="center";
              hh1.style.fontSize = "6em";
              allDivs[f].appendChild(hh1);
              var flag=false;
          if(cpu.includes(0) && cpu.includes(4) && cpu.includes(8))
          { 
            flag=true;;
          }
          else if(cpu.includes(2) && cpu.includes(4) && cpu.includes(6))
          {
            alert("Computer Won!");
            flag=true;;
          }

          else if(cpu.includes(0) && cpu.includes(1) && cpu.includes(2))
          { 
            flag=true;;
          }
          else if(cpu.includes(3) && cpu.includes(4) && cpu.includes(5))
          { 
            flag=true;;
          }
          else if(cpu.includes(6) && cpu.includes(7) && cpu.includes(8))
          { 
            flag=true;;
          }
          else if(cpu.includes(0) && cpu.includes(3) && cpu.includes(6))
          {
            alert("Computer Won!");
            flag=true;;
          }
          else if(cpu.includes(1) && cpu.includes(4) && cpu.includes(7))
          { 
            flag=true;;
          }

          else if(cpu.includes(2) && cpu.includes(5) && cpu.includes(7))
          { 
            flag=true;;
          }

          if(flag)
          {
            document.getElementById("result-header").innerText="Computer Won!";
          }

          break;
        }
      }   
    }
  }