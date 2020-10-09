var database = firebase.database();
const dbRef = database.ref("waterControl");

var pumpSystem = new Vue({
    el:"#pumpSystem",
    data:{
        status:'',
        stat:[],
    },
    methods:{
        updateStatus:function(){
            dbRef.child("pump").update({pumpStatus:!this.status})
        }

    },
    created(){
        dbRef.on('child_added',snapshot=>{
            this.stat.push(snapshot.val())
            console.log(snapshot.val())
            this.status = this.stat[(this.stat.length)-1].pumpStatus;
            
        })
        dbRef.on('child_changed',snapshot=>{
            const updateStatus="pump"
            updateStatus.pumpStatus = snapshot.val().pumpStatus
            this.status = snapshot.val().pumpStatus
            console.log(snapshot.val())
        })
    }

});