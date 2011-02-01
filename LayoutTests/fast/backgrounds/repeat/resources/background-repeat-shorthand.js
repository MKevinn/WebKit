description('This layout test used to test that correct shorthand name ' + 
            'is returned for background-repeat-x, ' +
            'background-repeat-y, background-position-x, background-position-y, ' +
            '-webkit-mask-repeat-x,-webkit-mask-repeat-y, -webkit-mask-position-x, ' +
            '-webkit-mask-position-y when corresponding shorthand is used in the style ' +
            'declaration. It tests regression described in ' +
            '<a href="https://bugs.webkit.org/show_bug.cgi?id=28972">this bug</a>.' +
            ' Now that access to non author stylesheet is blocked, we should instead' +
            ' get null when accessing the css rules on that object.');

function getShorthand(longhand, iconId)
{
    var iconDiv = document.getElementById(iconId);
    var rules = window.getMatchedCSSRules(iconDiv,'',false);
    return rules[1].style.getPropertyShorthand(longhand);
}

shouldThrow('getShorthand("background-repeat-x", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("background-repeat-y", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("background-repeat", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');

shouldThrow('getShorthand("background-position-x", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("background-position-y", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("background-position", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');

shouldThrow('getShorthand("-webkit-mask-repeat-x", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("-webkit-mask-repeat-y", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("-webkit-mask-repeat", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');

shouldThrow('getShorthand("-webkit-mask-position-x", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("-webkit-mask-position-y", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("-webkit-mask-repeat", "icon1")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');


shouldThrow('getShorthand("background-repeat-x", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("background-repeat-y", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("background-repeat", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');

shouldThrow('getShorthand("background-position-x", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("background-position-y", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("background-position", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');

shouldThrow('getShorthand("-webkit-mask-repeat-x", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("-webkit-mask-repeat-y", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("-webkit-mask-repeat", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');

shouldThrow('getShorthand("-webkit-mask-position-x", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("-webkit-mask-position-y", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');
shouldThrow('getShorthand("-webkit-mask-repeat", "icon2")', '"TypeError: Result of expression \'rules[1]\' [undefined] is not an object."');


var successfullyParsed = true;
