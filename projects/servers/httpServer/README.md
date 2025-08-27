# Overview

1. Learning materials for http: 

<table style:="width:100%">
    <tr>
        <th>Title</th>    
        <th>Status</th>    
        <th>Link</th>    
        <th>Useful Information</th>    
    </tr>
    <tr>
        <td>Protocol Details</td>    
        <td>In Progress</td>    
        <td>https://www.rfc-editor.org/rfc/rfc9110.html?</td>    
        <td> - </td>    
    </tr>
    <tr>
        <td>Socket Programming</td>    
        <td>In Progress</td>    
        <td>https://beej.us/guide/bgnet/?</td>    
        <td>Chapter 6 for TCP server</td>    
    </tr>
</table>

2. Ideas to pursue:

<table style:="width100%">
    <tr>
        <th>Idea</th>
        <th>Status</th>
    </tr>
    <tr>
        <td>Time difference in connection and response speed in ClientClass in C and Java Implementation</td>
        <td>Researching time classes</td>
    </tr>
</table>
        
a) Idea "response speed"
a.1) If we write a shell script or python script that compiles, executes and measures the time until it's finished, then compile time will interfere with measurement
a.2)    When using java timer and C timer classes respectively. Those have different implementations and will impact compilation and runtime as well.
        They only measure during runtime though, so we will use them for now and check if we can make it even fairer.
