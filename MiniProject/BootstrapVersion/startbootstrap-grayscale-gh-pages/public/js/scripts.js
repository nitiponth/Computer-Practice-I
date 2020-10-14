/*!
    * Start Bootstrap - Grayscale v6.0.2 (https://startbootstrap.com/themes/grayscale)
    * Copyright 2013-2020 Start Bootstrap
    * Licensed under MIT (https://github.com/StartBootstrap/startbootstrap-grayscale/blob/master/LICENSE)
    */
(function ($) {
    "use strict"; // Start of use strict

    // Smooth scrolling using jQuery easing
    $('a.js-scroll-trigger[href*="#"]:not([href="#"])').click(function () {
        if (
            location.pathname.replace(/^\//, "") ==
            this.pathname.replace(/^\//, "") &&
            location.hostname == this.hostname
        ) {
            var target = $(this.hash);
            target = target.length
                ? target
                : $("[name=" + this.hash.slice(1) + "]");
            if (target.length) {
                $("html, body").animate(
                    {
                        scrollTop: target.offset().top - 70,
                    },
                    1000,
                    "easeInOutExpo"
                );
                return false;
            }
        }
    });

    // Closes responsive menu when a scroll trigger link is clicked
    $(".js-scroll-trigger").click(function () {
        $(".navbar-collapse").collapse("hide");
    });

    // Activate scrollspy to add active class to navbar items on scroll
    $("body").scrollspy({
        target: "#mainNav",
        offset: 100,
    });

    // Collapse Navbar
    var navbarCollapse = function () {
        if ($("#mainNav").offset().top > 100) {
            $("#mainNav").addClass("navbar-shrink");
        } else {
            $("#mainNav").removeClass("navbar-shrink");
        }
    };
    // Collapse now if page is not at top
    navbarCollapse();
    // Collapse the navbar when page is scrolled
    $(window).scroll(navbarCollapse);
})(jQuery); // End of use strict

var database = firebase.database();
const dbStatusRef = database.ref("waterControl");
const webClickRef = database.ref("webClick");


var pumpSystem = new Vue({
    el: "#pumpSystem",
    data: {
        status: '',
        initTime:'',
        finTime:'',
        stat: [],
    },
    methods: {
        updateStatus: function () {
            dbStatusRef.child("pump").update({ pumpStatus: !this.status })
            webClickRef.child("clicked").update({ webClick: true })
        },
        updateTime: function(){
            dbStatusRef.child("time").update({initialTime: parseInt(this.initTime)})
            dbStatusRef.child("time").update({finalTime: parseInt(this.finTime)})
            this.initTime = ''
            this.finTime = ''
            alert("Set Auto watering time");
        }

    },
    created() {
        dbStatusRef.on('child_added', snapshot => {
            this.stat.push(snapshot.val())
            console.log(snapshot.val())
            this.status = this.stat[(this.stat.length) - 1].pumpStatus;

        })
        dbStatusRef.on('child_changed', snapshot => {
            const updateStatus = "pump"
            updateStatus.pumpStatus = snapshot.val().pumpStatus
            this.status = snapshot.val().pumpStatus
            console.log(snapshot.val())
        })
    }

});

var dbDate = "newLog/";
const dbLogRef = database.ref(dbDate);

var logSystem = new Vue({
    el: "#logSystem",
    data: {
        log: [],
        count: 0
    },
    filters: {
        reverse: function (array) {
            return array.slice().reverse()
        }
    },
    created() {
        dbLogRef.on('child_added', snapshot => {
            if (snapshot.val().updateFinish) {
                this.log.push(snapshot.val())
                console.log(snapshot.val())
            }
        })
        dbLogRef.on('child_changed', snapshot => {
            if (snapshot.val().updateFinish) {
                this.log.push(snapshot.val())
                console.log(snapshot.val())
            }
        })
    },
});
